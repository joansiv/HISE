/*  ===========================================================================
*
*   This file is part of HISE.
*   Copyright 2016 Christoph Hart
*
*   HISE is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   HISE is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with HISE.  If not, see <http://www.gnu.org/licenses/>.
*
*   Commercial licenses for using HISE in an closed source project are
*   available on request. Please visit the project's website to get more
*   information about commercial licensing:
*
*   http://www.hise.audio/
*
*   HISE is based on the JUCE library,
*   which must be separately licensed for closed source applications:
*
*   http://www.juce.com
*
*   ===========================================================================
*/

#ifndef PROCESSORINTERFACES_H_INCLUDED
#define PROCESSORINTERFACES_H_INCLUDED

namespace hise { using namespace juce;

/** A Processor that uses a Table.
*	@ingroup processor_interfaces
*
*	If your Processor uses a Table object for anything, you can subclass it from this interface class and use its Table. 
*	*/
class LookupTableProcessor
{
public:

	struct ProcessorValueConverter
	{
		ProcessorValueConverter() :
			converter(Table::getDefaultTextValue),
			processor(nullptr)
		{};

		ProcessorValueConverter(const Table::ValueTextConverter& c, Processor* p) :
			converter(c),
			processor(p)
		{};

		ProcessorValueConverter(Processor* p) :
			converter(Table::getDefaultTextValue),
			processor(p)
		{};

		
		bool operator==(const ProcessorValueConverter& other) const
		{
			return other.processor == processor;
		}

		explicit operator bool() const
		{
			return processor != nullptr;
		}

		String getDefaultTextValue(float input)
		{
			if (processor.get())
				return converter(input);
			else
				return Table::getDefaultTextValue(input);
		}

		Table::ValueTextConverter converter;
		WeakReference<Processor> processor;

	private:

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessorValueConverter);
	};

	// ================================================================================================================

	struct TableChangeBroadcaster : public SafeChangeBroadcaster
	{
        TableChangeBroadcaster()
        {
            
        }
        
		SpinLock lock;

		WeakReference<Table> table;
		float tableIndex;
	};

	LookupTableProcessor();
	virtual ~LookupTableProcessor();

	//SET_PROCESSOR_CONNECTOR_TYPE_ID("LookupTableProcessor");

	// ================================================================================================================

	/** Overwrite this method and return the table for the supplied index.
	*
	*	If you only have one table, ignore this parameter.*/
	virtual Table *getTable(int tableIndex) const = 0;

	/** Overwrite this and return the number of tables that this processor uses.
	*
	*	It assumes one table so if you do have one table, you don't need to do anything...
	*/
	virtual int getNumTables() const { return 1; }

	/** Adds a listener to this processor. */
	void addTableChangeListener(SafeChangeListener *listener);;

	/** Removes a listener from this processor. */
	void removeTableChangeListener(SafeChangeListener *listener);;

	/** Call this method whenever the table index is changed and all connected tables will receive a change message.
	*
	*	You can dynamic_cast the broadcaster to a LookupTableProcessor::TableChangeBroadcaster and use this code:
	*
	*		if(dynamic_cast<LookupTableProcessor::TableChangeBroadcaster*>(b) != nullptr)
	*		{
	*			dynamic_cast<LookupTableProcessor::TableChangeBroadcaster*>(b)->table->setDisplayedIndex(tableValue);
	*		}
	*/
	void sendTableIndexChangeMessage(bool sendSynchronous, Table *table, float tableIndex);

	void addYValueConverter(const Table::ValueTextConverter& converter, Processor* p)
	{
		if (p == dynamic_cast<Processor*>(this))
			defaultYConverter = converter;
		else
		{
			for (int i = 0; i < yConverters.size(); i++)
			{
				auto thisP = yConverters[i]->processor.get();

				if (thisP == nullptr || thisP == p)
					yConverters.remove(i--);
			}

			yConverters.add(new ProcessorValueConverter(converter, p ));
		}

		updateYConverters();
	}

	void refreshYConvertersAfterRemoval()
	{
		for (int i = 0; i < yConverters.size(); i++)
		{
			auto thisP = yConverters[i]->processor.get();

			if (thisP == nullptr)
				yConverters.remove(i--);
		}

		updateYConverters();
	}

protected:

	Table::ValueTextConverter defaultYConverter = Table::getDefaultTextValue;

private:

	void updateYConverters()
	{
		const auto cToUse = yConverters.size() == 1 ? yConverters.getFirst()->converter : defaultYConverter;

		for (int i = 0; i < getNumTables(); i++)
			getTable(i)->setYTextConverterRaw(cToUse);
	}

	OwnedArray<ProcessorValueConverter> yConverters;
	

	TableChangeBroadcaster tableChangeBroadcaster;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LookupTableProcessor);

	// ================================================================================================================
};

class SliderPackData;

/** A Processor that uses a SliderPack. 
*	@ingroup processor_interfaces
*
*	It is a pure virtual class interface without member data to prevent the Diamond of Death.
*/
class SliderPackProcessor
{
public:

	// ================================================================================================================

	SliderPackProcessor() {};
	virtual ~SliderPackProcessor() {};

	//SET_PROCESSOR_CONNECTOR_TYPE_ID("SliderPackProcessor");

	/** Overwrite this and return the const SliderPackData member from your subclassed Processor. */
	virtual SliderPackData *getSliderPackData(int index) = 0;

	/** Overwrite this and return the SliderPackData member from your subclassed Processor. */
	virtual const SliderPackData *getSliderPackData(int index) const = 0;

	virtual int getNumSliderPacks() const { return 1; }

private:

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SliderPackProcessor);

	// ================================================================================================================
};



/** A Processor that uses an audio sample.
*	@ingroup processor_interfaces
*
*	If you want to use a audio sample from the pool, subclass your Processor from this and you will get some nice tools:
*
*	- automatic memory management (samples are reference counted and released on destruction)
*	- import / export sample properties in the RestorableObject methods
*	- handle the thumbnail cache
*	- designed to work with an AudioSampleBufferComponent
*
*	In order to use this class with a AudioSampleBufferComponent, just follow these steps:
*
*	1. Create a AudioSampleBufferComponent and use the method getCache() in the constructor.
*	2. Set the reference to the AudioSampleBuffer with AudioSampleBufferComponent::setAudioSampleBuffer();
*	3. Add the AudioSampleBuffer as ChangeListener (and remove it in the destructor!)
*	4. Add an AreaListener to the AudioSampleBufferComponent and call setRange() and setLoadedFile in the rangeChanged() callback
*/
class AudioSampleProcessor: public PoolBase::Listener
{
public:

	// ================================================================================================================

	enum SyncToHostMode
	{
		FreeRunning = 1,
		OneBeat,
		TwoBeats,
		OneBar,
		TwoBars,
		FourBars
	};

	/** Automatically releases the sample in the pool. */
	virtual ~AudioSampleProcessor();

	// ================================================================================================================

	/** Call this method within your exportAsValueTree method to store the sample settings. */
	void saveToValueTree(ValueTree &v) const;;

	/** Call this method within your restoreFromValueTree() method to load the sample settings. */
	void restoreFromValueTree(const ValueTree &v);

	/** This loads the file from disk (or from the pool, if existing and loadThisFile is false. */
	void setLoadedFile(const String &fileName, bool loadThisFile = false, bool forceReload = false);

	/** Sets the sample range that should be used in the plugin.
	*
	*	This is called automatically if a AudioSampleBufferComponent is set up correctly.
	*/
	void setRange(Range<int> newSampleRange);

	/** Returns the range of the sample. */
	Range<int> getRange() const
	{ 
		return sampleRange; 
	};

	void poolEntryReloaded(PoolReference referenceThatWasChanged) override;

	int getTotalLength() const { return data ? data.getData()->getNumSamples() : 0; };

	/** Returns a const pointer to the audio sample buffer.
	*
	*	The pointer references a object from a AudioSamplePool and should be valid as long as the pool is not cleared. */
	const AudioSampleBuffer *getBuffer() 
	{
		if(data)
			return data.getData(); 

		return &fallback;
	};

	const AudioSampleBuffer *getSampleBuffer() const 
	{ 
		if (data)
			return data.getData();

		return &fallback;
	};

	void setLoopFromMetadata(const var& md);

	void setUseLoop(bool shouldUseLoop)
	{
		if (useLoop != shouldUseLoop)
		{
			useLoop = shouldUseLoop;

			if (useLoop && loopRange.isEmpty())
				loopRange = sampleRange;
		}
	}

	bool isUsingLoop() const noexcept
	{
		return useLoop;
	}

	/** Returns the filename that was loaded.
	*
	*	It is possible that the file does not exist on your system:
	*	If you restore a pool completely from a ValueTree, it still uses the absolute filename as identification.
	*/
	String getFileName() const { return data.getRef().getReferenceString(); };

	/** Overwrite this method and do whatever needs to be done when the selected range changes. */
	virtual void rangeUpdated() {};

	/** Overwrite this method and do whatever needs to be done when a new file is loaded.
	*
	*	You don't need to call setLoadedFile(), but if you got some internal stuff going on, this is the place.
	*/
	virtual void newFileLoaded() {};

	double getSampleRateForLoadedFile() const { return sampleRateOfLoadedFile; }

	virtual const CriticalSection& getFileLock() const = 0;

	Range<int> getActualRange() const
	{
		if (isUsingLoop())
		{
			int x1 = sampleRange.getStart();
			int x2 = jmin<int>(sampleRange.getEnd(), loopRange.getEnd());

			return { x1, x2 };
		}
		else
		{
			return sampleRange;
		}
	}

	Range<int> getLoopRange()
	{
		return loopRange;
	}

	

	
protected:

	/** Call this constructor within your subclass constructor. */
	AudioSampleProcessor(Processor *p);

	PooledAudioFile data;

	Range<int> sampleRange;
	int length;

	Range<int> loopRange;

	

	double sampleRateOfLoadedFile;

	bool useLoop = false;

	WeakReference<AudioSampleBufferPool> currentPool;

private:

	

	AudioSampleBuffer fallback;

	int getConstrainedLoopValue(String metadata);
	

	// ================================================================================================================

	
	MainController *mc;

	// ================================================================================================================
};



/** A Processor that has a dynamic size of child processors.
*	@ingroup processor_interfaces
*
*	If your Processor has more than a fixed amount of internal child processors, derive it from this class, write a Chain::Handler subclass with all
*	needed operations and you can add / delete Processors on runtime.
*
*	You might want to overwrite the Processors functions getNumChildProcessors() and getChildProcessor() with the handlers methods (handle internal chains manually)
*	This allows the restoreState function to only clear the dynamic list of processors.
*/
class Chain
{
public:

	// ====================================================================================================================

	/**
	*
	*	Subclass this, put it in your subclassed Chain and return a member object of the chain in Chain::getHandler().
	*/
	class Handler
	{
	public:

		struct Listener
		{
			enum EventType
			{
				ProcessorAdded,
				ProcessorDeleted,
				ProcessorOrderChanged,
				Cleared,
				numEventTypes
			};

            virtual ~Listener() {};
            
			virtual void processorChanged(EventType t, Processor* p) = 0;

			JUCE_DECLARE_WEAK_REFERENCEABLE(Listener);
		};

		// ================================================================================================================

		virtual ~Handler() {};

		/** Adds a new processor to the chain. It must be owned by the chain. */
		virtual void add(Processor *newProcessor, Processor *siblingToInsertBefore) = 0;

		/** Deletes a processor from the chain. */
		virtual void remove(Processor *processorToBeRemoved, bool deleteProcessor=true) = 0;

		/** Returns the processor at the index. */
		virtual Processor *getProcessor(int processorIndex) = 0;

		virtual const Processor *getProcessor(int processorIndex) const = 0;

		/** Overwrite this method and implement a move operation. */
		virtual void moveProcessor(Processor* /*processorInChain*/, int /*delta*/) {};

		/** Returns the amount of processors. */
		virtual int getNumProcessors() const = 0;

		/** Deletes all Processors in the Chain. */
		virtual void clear() = 0;

		void clearAsync(Processor* parent);

		void addListener(Listener* l)
		{
			listeners.addIfNotAlreadyThere(l);
		}

		void removeListener(Listener* l)
		{
			listeners.removeAllInstancesOf(l);
		}

		void addPostEventListener(Listener* l)
		{
			postEventlisteners.addIfNotAlreadyThere(l);
		}

		void removePostEventListener(Listener* l)
		{
			postEventlisteners.removeAllInstancesOf(l);
		}

		void notifyListeners(Listener::EventType t, Processor* p)
		{
			ScopedLock sl(listeners.getLock());

			for (auto l : listeners)
			{
				if (l != nullptr)
					l->processorChanged(t, p);
			}
		}

		void notifyPostEventListeners(Listener::EventType t, Processor* p)
		{
			ScopedLock sl(postEventlisteners.getLock());

			for (auto l : postEventlisteners)
			{
				if (l != nullptr)
					l->processorChanged(t, p);
			}
		}

	private:

		

		Array<WeakReference<Listener>, CriticalSection> listeners;
		Array<WeakReference<Listener>, CriticalSection> postEventlisteners;
	};

	// ===================================================================================================================

	/** Restores a Chain from a ValueTree. It creates all processors and restores their values. It returns false, if anything went wrong. */
	bool restoreChain(const ValueTree &v);

	/** Overwrite this and return the processor that owns this chain if it exists. */
	virtual Processor *getParentProcessor() = 0;

	/** Overwrite this and return the processor that owns this chain if it exists. */
	virtual const Processor *getParentProcessor() const = 0;

	/** return your subclassed Handler. */
	virtual Handler *getHandler() = 0;

	/** read only access to the Handler. */
	virtual const Handler *getHandler() const = 0;

	virtual ~Chain() {};

	/** Sets the FactoryType that will be used. */
	virtual void setFactoryType(FactoryType *newType) = 0;

	/** Returns the Factory type this processor is using. */
	virtual FactoryType *getFactoryType() const = 0;

	// ================================================================================================================


};

#define ADD_NAME_TO_TYPELIST(x) (typeNames.add(FactoryType::ProcessorEntry(x::getClassType(), x::getClassName())))

/** This interface class lets the MainController do its work.
*	@ingroup factory
*
*	You can tell a Processor (which should also be a Chain to make sense) to use a specific FactoryType 
*	with Processor::setFactoryType(), which will then use it in its popup menu to create the possible Processors. 
*	Simply overwrite these two functions in your subclass:
*
*		Processor* createProcessor	(int typeIndex, const String &id);
*		const StringArray& getTypeNames() const;
*
*	A FactoryType constrains the number of creatable Processors by
*
*	- Type (will be defined by the subclass)
*	- Constrainer (can be added to a FactoryType and uses runtime information like parent processor etc.)
*
*/
class FactoryType
{
public:

	// ================================================================================================================

	/** A Constrainer objects can impose restrictions on a particular FactoryType
	*
	*	If you want to restrict the selection of possible Processor types, you can
	*	subclass this, overwrite allowType with your custom rules and call
	*	setConstrainer() on the FactoryType you want to limit.
	*/
	class Constrainer: public BaseConstrainer
	{
	public:

		virtual ~Constrainer() {};

		/** Overwrite this and return true if the FactoryType can create this Processor and false, if not. */
		virtual bool allowType(const Identifier &typeName) = 0;

	};

	// ================================================================================================================

	/** a simple POD which contains the id and the name of a Processor type. */
	struct ProcessorEntry
	{
		ProcessorEntry(const Identifier t, const String &n);;
		ProcessorEntry() {};

		Identifier type;
		String name;
	};

	// ================================================================================================================

	/** Creates a Factory type.  */
	FactoryType(Processor *owner_);;
	virtual ~FactoryType();

	// ================================================================================================================

	/** Fills a popupmenu with all allowed processor types.
	*
	*	You can pass in a startIndex, if you overwrite this method for nested calls.
	*
	*	It returns the last index that can be used for the next menus.
	*/
	virtual int fillPopupMenu(PopupMenu &m, int startIndex = 1);;

	/** Overwrite this function and return a processor of the specific type index. */
	virtual Processor *createProcessor(int typeIndex, const String &ProcessorId) = 0;

	/** Returns the typeName using the result from the previously created popupmenu. */
	Identifier getTypeNameFromPopupMenuResult(int resultFromPopupMenu);

	/** Returns the typeName using the result from the previously created popupmenu. */
	String getNameFromPopupMenuResult(int resultFromPopupMenu);

	/** Returns the index of the type. */
	virtual int getProcessorTypeIndex(const Identifier &typeName) const;;

	/** Returns the number of Processors that this factory can create.
	*
	*	the rules defined in allowType are applied before counting the possible processors.
	*/
	virtual int getNumProcessors();

	const Processor *getOwnerProcessor() const { return owner.get(); };
	Processor *getOwnerProcessor()			   { return owner.get(); };

	/**	Checks if the type of the processor is found in the type name.
	*
	*	You can overwrite this and add more conditions (in this case, call the base class method first to keep things safe!
	*/
	virtual bool allowType(const Identifier &typeName) const;;

	/** Returns a unique ID for the new Processor.
	*
	*	It scans all Processors and returns something like "Processor12" if there are 11 other Processors with the same ID
	*/
	static String getUniqueName(Processor *id, String name = String());


	/** Returns a string array with all allowed types that this factory can produce. */
	virtual Array<ProcessorEntry> getAllowedTypes();;

	/** adds a Constrainer to a FactoryType. It will be owned by the FactoryType. You can pass nullptr. */
	virtual void setConstrainer(Constrainer *newConstrainer, bool ownConstrainer = true);

	Constrainer *getConstrainer();

	// ================================================================================================================

protected:

	/** This should only be overwritten by the subclasses. For external usage, use getAllowedTypes(). */
	virtual const Array<ProcessorEntry> &getTypeNames() const = 0;

	WeakReference<Processor> owner;

private:

	// iterates all child processors and counts the number of same IDs.
	static bool countProcessorsWithSameId(int &index, const Processor *p, 
										  Processor *processorToLookFor, const String &nameToLookFor);

	ScopedPointer<Constrainer> ownedConstrainer;
	Constrainer *constrainer;

	mutable bool baseClassCalled;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FactoryType);

	// ================================================================================================================
};

} // namespace hise

#endif  // PROCESSORINTERFACES_H_INCLUDED
