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
*   which also must be licenced for commercial applications:
*
*   http://www.juce.com
*
*   ===========================================================================
*/

namespace hise { using namespace juce;



// MidiList =====================================================================================================================

struct ScriptingObjects::MidiList::Wrapper
{
	API_VOID_METHOD_WRAPPER_1(MidiList, fill);
	API_VOID_METHOD_WRAPPER_0(MidiList, clear);
	API_METHOD_WRAPPER_1(MidiList, getValue);
	API_METHOD_WRAPPER_1(MidiList, getValueAmount);
	API_METHOD_WRAPPER_1(MidiList, getIndex);
	API_METHOD_WRAPPER_0(MidiList, isEmpty);
	API_METHOD_WRAPPER_0(MidiList, getNumSetValues);
	API_VOID_METHOD_WRAPPER_2(MidiList, setValue);
	API_VOID_METHOD_WRAPPER_1(MidiList, restoreFromBase64String);
	API_METHOD_WRAPPER_0(MidiList, getBase64String);
};

ScriptingObjects::MidiList::MidiList(ProcessorWithScriptingContent *p) :
ConstScriptingObject(p, 0)
{
	ADD_API_METHOD_1(fill);
	ADD_API_METHOD_0(clear);
	ADD_API_METHOD_1(getValue);
	ADD_API_METHOD_1(getValueAmount);
	ADD_API_METHOD_1(getIndex);
	ADD_API_METHOD_0(isEmpty);
	ADD_API_METHOD_0(getNumSetValues);
	ADD_API_METHOD_2(setValue);
	ADD_API_METHOD_1(restoreFromBase64String);
	ADD_API_METHOD_0(getBase64String);

	clear();
}

void ScriptingObjects::MidiList::assign(const int index, var newValue)			 { setValue(index, (int)newValue); }
int ScriptingObjects::MidiList::getCachedIndex(const var &indexExpression) const { return (int)indexExpression; }
var ScriptingObjects::MidiList::getAssignedValue(int index) const				 { return getValue(index); }

void ScriptingObjects::MidiList::fill(int valueToFill)
{
	for (int i = 0; i < 128; i++) data[i] = valueToFill;
	empty = false;
	numValues = 128;
}

void ScriptingObjects::MidiList::clear()
{
	fill(-1);
	empty = true;
	numValues = 0;
}

int ScriptingObjects::MidiList::getValue(int index) const
{
	if (index < 127 && index >= 0) return (int)data[index]; else return -1;
}

int ScriptingObjects::MidiList::getValueAmount(int valueToCheck)
{
	if (empty) return 0;

	int amount = 0;

	for (int i = 0; i < 128; i++)
	{
		if (data[i] == valueToCheck) amount++;
	}

	return amount;
}

int ScriptingObjects::MidiList::getIndex(int value) const
{
	if (empty) return -1;
	for (int i = 0; i < 128; i++)
	{
		if (data[i] == value)
		{
			return i;
		}
	}

	return -1;
}

void ScriptingObjects::MidiList::setValue(int index, int value)
{
	if (index >= 0 && index < 128)
	{
        if (value == -1)
		{
            if(data[index] != -1)
            {
                numValues--;
                if (numValues == 0) empty = true;
            }
		}
		else
		{
            if(data[index] == -1)
            {
                numValues++;
                empty = false;
            }
            
            data[index] = value;
		}
	}
}

String ScriptingObjects::MidiList::getBase64String() const
{
	MemoryOutputStream stream;
	Base64::convertToBase64(stream, data, sizeof(int) * 128);
	return stream.toString();
}

void ScriptingObjects::MidiList::restoreFromBase64String(String base64encodedValues)
{
	MemoryOutputStream stream(data, sizeof(int) * 128);
	Base64::convertFromBase64(stream, base64encodedValues);
}

void addScriptParameters(ConstScriptingObject* this_, Processor* p)
{
	DynamicObject::Ptr scriptedParameters = new DynamicObject();

	if (ProcessorWithScriptingContent* pwsc = dynamic_cast<ProcessorWithScriptingContent*>(p))
	{
		for (int i = 0; i < pwsc->getScriptingContent()->getNumComponents(); i++)
		{
			scriptedParameters->setProperty(pwsc->getScriptingContent()->getComponent(i)->getName(), var(i));
		}
	}

	this_->addConstant("ScriptParameters", var(scriptedParameters.get()));
}


struct ScriptingObjects::ScriptSliderPackData::Wrapper
{
	API_VOID_METHOD_WRAPPER_2(ScriptSliderPackData, setValue);
	API_VOID_METHOD_WRAPPER_1(ScriptSliderPackData, setNumSliders);
	API_METHOD_WRAPPER_1(ScriptSliderPackData, getValue);
	API_METHOD_WRAPPER_0(ScriptSliderPackData, getNumSliders);
	API_VOID_METHOD_WRAPPER_3(ScriptSliderPackData, setRange);
	
};

ScriptingObjects::ScriptSliderPackData::ScriptSliderPackData(ProcessorWithScriptingContent* pwsc) :
	ConstScriptingObject(pwsc, 0),
	data(pwsc->getMainController_()->getControlUndoManager(), pwsc->getMainController_()->getGlobalUIUpdater())
{
	data.setNumSliders(16);

	ADD_API_METHOD_2(setValue);
	ADD_API_METHOD_1(setNumSliders);
	ADD_API_METHOD_1(getValue);
	ADD_API_METHOD_0(getNumSliders);
	ADD_API_METHOD_3(setRange);
}

void ScriptingObjects::ScriptSliderPackData::rightClickCallback(const MouseEvent& e, Component *c)
{
#if USE_BACKEND
	SliderPack *s = new SliderPack(&data);

	const int numSliders = getNumSliders();



	int widthPerSlider = 16;

	if (numSliders > 64)
		widthPerSlider = 8;

	s->setSize((int)getNumSliders() * 16, 200);

	auto editor = GET_BACKEND_ROOT_WINDOW(c);

	MouseEvent ee = e.getEventRelativeTo(editor);

	editor->getRootFloatingTile()->showComponentInRootPopup(s, editor, ee.getMouseDownPosition());
#else
    ignoreUnused(e, c);
#endif
}

var ScriptingObjects::ScriptSliderPackData::getStepSize() const
{
	return data.getStepSize();
}

void ScriptingObjects::ScriptSliderPackData::setNumSliders(var numSliders)
{
	data.setNumSliders(numSliders);
}

int ScriptingObjects::ScriptSliderPackData::getNumSliders() const
{
	return data.getNumSliders();
}

void ScriptingObjects::ScriptSliderPackData::setValue(int sliderIndex, float value)
{
	data.setValue(sliderIndex, value, sendNotification);
}

float ScriptingObjects::ScriptSliderPackData::getValue(int index) const
{
	return data.getValue((int)index);
}

void ScriptingObjects::ScriptSliderPackData::setRange(double minValue, double maxValue, double stepSize)
{
	data.setRange(minValue, maxValue, stepSize);
}

// ScriptingModulator ===========================================================================================================

struct ScriptingObjects::ScriptingModulator::Wrapper
{
	API_VOID_METHOD_WRAPPER_2(ScriptingModulator, setAttribute);
    API_METHOD_WRAPPER_1(ScriptingModulator, getAttribute);
	API_METHOD_WRAPPER_0(ScriptingModulator, getNumAttributes);
	API_VOID_METHOD_WRAPPER_1(ScriptingModulator, setBypassed);
	API_METHOD_WRAPPER_0(ScriptingModulator, isBypassed);
	API_VOID_METHOD_WRAPPER_1(ScriptingModulator, setIntensity);
	API_METHOD_WRAPPER_0(ScriptingModulator, getIntensity);
	API_METHOD_WRAPPER_0(ScriptingModulator, getCurrentLevel);
	API_METHOD_WRAPPER_0(ScriptingModulator, exportState);
	API_VOID_METHOD_WRAPPER_1(ScriptingModulator, restoreState);
	API_VOID_METHOD_WRAPPER_1(ScriptingModulator, restoreScriptControls);
	API_METHOD_WRAPPER_0(ScriptingModulator, exportScriptControls);
	API_METHOD_WRAPPER_3(ScriptingModulator, addModulator);
    API_METHOD_WRAPPER_1(ScriptingModulator, getModulatorChain);
    
	API_METHOD_WRAPPER_3(ScriptingModulator, addGlobalModulator);
	API_METHOD_WRAPPER_3(ScriptingModulator, addStaticGlobalModulator);
	API_METHOD_WRAPPER_0(ScriptingModulator, asTableProcessor);
	API_METHOD_WRAPPER_0(ScriptingModulator, getId);
};

ScriptingObjects::ScriptingModulator::ScriptingModulator(ProcessorWithScriptingContent *p, Modulator *m_) :
ConstScriptingObject(p, m_ != nullptr ? m_->getNumParameters() + 1 : 1),
mod(m_),
m(nullptr),
moduleHandler(m_, dynamic_cast<JavascriptProcessor*>(p))
{
	if (mod != nullptr)
	{
		m = dynamic_cast<Modulation*>(m_);

		setName(mod->getId());

		addScriptParameters(this, mod.get());

		for (int i = 0; i < mod->getNumParameters(); i++)
		{
			addConstant(mod->getIdentifierForParameterIndex(i).toString(), var(i));
		}
	}
	else
	{
		setName("Invalid Modulator");
	}

	ADD_API_METHOD_0(getId);
	ADD_API_METHOD_2(setAttribute);
	ADD_API_METHOD_1(setBypassed);
	ADD_API_METHOD_0(isBypassed);
	ADD_API_METHOD_1(setIntensity);
	ADD_API_METHOD_0(getIntensity);
    ADD_API_METHOD_1(getAttribute);
	ADD_API_METHOD_0(getCurrentLevel);
	ADD_API_METHOD_0(exportState);
	ADD_API_METHOD_1(restoreState);
	ADD_API_METHOD_0(getNumAttributes);
	ADD_API_METHOD_1(restoreScriptControls);
	ADD_API_METHOD_0(exportScriptControls);
	ADD_API_METHOD_3(addModulator);
    ADD_API_METHOD_1(getModulatorChain);
    
	ADD_API_METHOD_3(addGlobalModulator);
	ADD_API_METHOD_3(addStaticGlobalModulator);
	ADD_API_METHOD_0(asTableProcessor);
}

String ScriptingObjects::ScriptingModulator::getDebugName() const
{
	if (objectExists() && !objectDeleted())
		return mod->getId();

	return String("Invalid");
}

String ScriptingObjects::ScriptingModulator::getDebugValue() const
{
	if (objectExists() && !objectDeleted())
		return String(mod->getOutputValue(), 2);

	return "0.0";
}

int ScriptingObjects::ScriptingModulator::getCachedIndex(const var &indexExpression) const
{
	if (checkValidObject())
	{
		Identifier id(indexExpression.toString());

		for (int i = 0; i < mod->getNumParameters(); i++)
		{
			if (id == mod->getIdentifierForParameterIndex(i)) return i;
		}
		return -1;
	}
	else
	{
		throw String("Modulator does not exist");
	}
}

void ScriptingObjects::ScriptingModulator::assign(const int index, var newValue)
{
	setAttribute(index, (float)newValue);
}

var ScriptingObjects::ScriptingModulator::getAssignedValue(int /*index*/) const
{
	return 1.0; // Todo...
}

String ScriptingObjects::ScriptingModulator::getId() const
{
	if (checkValidObject())
		return mod->getId();

	return String();
}

void ScriptingObjects::ScriptingModulator::setAttribute(int index, float value)
{
	if (checkValidObject())
		mod->setAttribute(index, value, sendNotification);
}

float ScriptingObjects::ScriptingModulator::getAttribute(int parameterIndex)
{
    if (checkValidObject())
    {
        return mod->getAttribute(parameterIndex);
    }

	return 0.0f;
}

int ScriptingObjects::ScriptingModulator::getNumAttributes() const
{
	if (checkValidObject())
	{
		return mod->getNumParameters();
	}

	return 0;
}

void ScriptingObjects::ScriptingModulator::setBypassed(bool shouldBeBypassed)
{
	if (checkValidObject())
	{
		mod->setBypassed(shouldBeBypassed, sendNotification);
		mod->sendChangeMessage();
	}
}



bool ScriptingObjects::ScriptingModulator::isBypassed() const
{
	if (checkValidObject())
	{
		return mod->isBypassed();
	}

	return false;
}

void ScriptingObjects::ScriptingModulator::doubleClickCallback(const MouseEvent &, Component* componentToNotify)
{
#if USE_BACKEND
	if (objectExists() && !objectDeleted())
	{
		auto *editor = GET_BACKEND_ROOT_WINDOW(componentToNotify);

		Processor *p = ProcessorHelpers::getFirstProcessorWithName(editor->getMainSynthChain(), mod->getId());

		if (p != nullptr)
		{
			editor->getMainPanel()->setRootProcessorWithUndo(p);
		}
	}
#else 
	ignoreUnused(componentToNotify);
#endif
}

void ScriptingObjects::ScriptingModulator::rightClickCallback(const MouseEvent& e, Component* t)
{
	Helpers::showProcessorEditorPopup(e, t, mod);
}

void ScriptingObjects::ScriptingModulator::setIntensity(float newIntensity)
{
	if (checkValidObject())
	{
		auto mode = m->getMode();

		if (mode == Modulation::GainMode)
		{
			const float value = jlimit<float>(0.0f, 1.0f, newIntensity);
			m->setIntensity(value);

			mod.get()->sendChangeMessage();
		}
		else if(mode == Modulation::PitchMode)
		{
			const float value = jlimit<float>(-12.0f, 12.0f, newIntensity);
			const float pitchFactor = value / 12.0f;

			m->setIntensity(pitchFactor);

			mod.get()->sendChangeMessage();
		}
		else
		{
			const float value = jlimit<float>(-1.0f, 1.0f, newIntensity);

			m->setIntensity(value);

			mod.get()->sendChangeMessage();
		}
	}
};



float ScriptingObjects::ScriptingModulator::getIntensity() const
{
	if (checkValidObject())
	{
		if (m->getMode() == Modulation::PitchMode)
		{
			return dynamic_cast<const Modulation*>(mod.get())->getIntensity() * 12.0f;
		}
		else
		{
			return dynamic_cast<const Modulation*>(mod.get())->getIntensity();
		}
	}

	return 0.0f;
}

float ScriptingObjects::ScriptingModulator::getCurrentLevel()
{
	if (checkValidObject())
	{
		return m->getProcessor()->getDisplayValues().outL;
	}
	
	return 0.f;
}

String ScriptingObjects::ScriptingModulator::exportState()
{
	if (checkValidObject())
	{
		return ProcessorHelpers::getBase64String(mod);
	}

	return String();
}

void ScriptingObjects::ScriptingModulator::restoreState(String base64State)
{
	if (checkValidObject())
	{
		ProcessorHelpers::restoreFromBase64String(mod, base64State);
	}
}

String ScriptingObjects::ScriptingModulator::exportScriptControls()
{
	if (dynamic_cast<ProcessorWithScriptingContent*>(mod.get()) == nullptr)
	{
		reportScriptError("exportScriptControls can only be used on Script Processors");
	}

	if (checkValidObject())
	{
		return ProcessorHelpers::getBase64String(mod, false, true);
	}

	return String();
}

void ScriptingObjects::ScriptingModulator::restoreScriptControls(String base64Controls)
{
	if (dynamic_cast<ProcessorWithScriptingContent*>(mod.get()) == nullptr)
	{
		reportScriptError("restoreScriptControls can only be used on Script Processors");
	}

	if (checkValidObject())
	{
		ProcessorHelpers::restoreFromBase64String(mod, base64Controls, true);
	}
}

var ScriptingObjects::ScriptingModulator::addModulator(var chainIndex, var typeName, var modName)
{
	if (checkValidObject())
	{
		ModulatorChain *c = dynamic_cast<ModulatorChain*>(mod->getChildProcessor(chainIndex));

		if (c == nullptr)
			reportScriptError("Modulator Chain with index " + chainIndex.toString() + " does not exist");
		
		Processor* p = moduleHandler.addModule(c, typeName, modName, -1);

		if (p != nullptr)
		{
			auto newMod = new ScriptingObjects::ScriptingModulator(getScriptProcessor(), dynamic_cast<Modulator*>(p));
			return var(newMod);
		}
		
	}

	return var();
}
    
var ScriptingObjects::ScriptingModulator::getModulatorChain(var chainIndex)
{
    if (checkValidObject())
    {
        auto c = dynamic_cast<Modulator*>(mod->getChildProcessor(chainIndex));
        
        if (c == nullptr)
            reportScriptError("Modulator Chain with index " + chainIndex.toString() + " does not exist");
            
        auto modChain = new ScriptingModulator(getScriptProcessor(), c);
            
        return var(modChain);
    }
    else
    {
        return var();
    }
}

var ScriptingObjects::ScriptingModulator::addGlobalModulator(var chainIndex, var globalMod, String modName)
{
	if (checkValidObject())
	{
		if (auto gm = dynamic_cast<ScriptingModulator*>(globalMod.getObject()))
		{
			ModulatorChain *c = dynamic_cast<ModulatorChain*>(mod->getChildProcessor(chainIndex));

			if (c == nullptr)
				reportScriptError("Modulator Chain with index " + chainIndex.toString() + " does not exist");

			auto p = moduleHandler.addAndConnectToGlobalModulator(c, gm->getModulator(), modName);

			if (p != nullptr)
			{
				auto newMod = new ScriptingObjects::ScriptingModulator(getScriptProcessor(), p);
				return var(newMod);
			}
			
		}
	}

	return var();
}

var ScriptingObjects::ScriptingModulator::addStaticGlobalModulator(var chainIndex, var timeVariantMod, String modName)
{
	if (checkValidObject())
	{
		if (auto gm = dynamic_cast<ScriptingModulator*>(timeVariantMod.getObject()))
		{
			ModulatorChain *c = dynamic_cast<ModulatorChain*>(mod->getChildProcessor(chainIndex));

			if (c == nullptr)
				reportScriptError("Modulator Chain with index " + chainIndex.toString() + " does not exist");

			auto p = moduleHandler.addAndConnectToGlobalModulator(c, gm->getModulator(), modName, true);

			if (p != nullptr)
			{
				auto newMod = new ScriptingObjects::ScriptingModulator(getScriptProcessor(), p);
				return var(newMod);
			}

		}
	}

	return var();
}

var ScriptingObjects::ScriptingModulator::asTableProcessor()
{
	if (checkValidObject())
	{
		auto ltp = dynamic_cast<LookupTableProcessor*>(mod.get());

		if (ltp == nullptr)
			return var(); // don't complain here, handle it on scripting level

		auto t = new ScriptingTableProcessor(getScriptProcessor(), ltp);
			return var(t);
	}

	auto t = new ScriptingObjects::ScriptingTableProcessor(getScriptProcessor(), nullptr);
	return var(t);
}

// ScriptingEffect ==============================================================================================================

struct ScriptingObjects::ScriptingEffect::Wrapper
{
	API_VOID_METHOD_WRAPPER_2(ScriptingEffect, setAttribute);
    API_METHOD_WRAPPER_1(ScriptingEffect, getAttribute);
	API_METHOD_WRAPPER_0(ScriptingEffect, getNumAttributes);
	API_VOID_METHOD_WRAPPER_1(ScriptingEffect, setBypassed);
	API_METHOD_WRAPPER_0(ScriptingEffect, exportState);
	API_METHOD_WRAPPER_1(ScriptingEffect, getCurrentLevel);
	API_VOID_METHOD_WRAPPER_1(ScriptingEffect, restoreState);
	API_VOID_METHOD_WRAPPER_1(ScriptingEffect, restoreScriptControls);
	API_METHOD_WRAPPER_0(ScriptingEffect, exportScriptControls);
	API_METHOD_WRAPPER_3(ScriptingEffect, addModulator);
	API_METHOD_WRAPPER_3(ScriptingEffect, addGlobalModulator);
	API_METHOD_WRAPPER_1(ScriptingEffect, getModulatorChain);
	API_METHOD_WRAPPER_3(ScriptingEffect, addStaticGlobalModulator);
	API_METHOD_WRAPPER_0(ScriptingEffect, getId);
};

ScriptingObjects::ScriptingEffect::ScriptingEffect(ProcessorWithScriptingContent *p, EffectProcessor *fx) :
ConstScriptingObject(p, fx != nullptr ? fx->getNumParameters()+1 : 1),
effect(fx),
moduleHandler(fx, dynamic_cast<JavascriptProcessor*>(p))
{
	if (fx != nullptr)
	{
		setName(fx->getId());

		addScriptParameters(this, effect.get());

		for (int i = 0; i < fx->getNumParameters(); i++)
		{
			addConstant(fx->getIdentifierForParameterIndex(i).toString(), var(i));
		}
	}
	else
	{
		setName("Invalid Effect");
	}

	ADD_API_METHOD_0(getId);
	ADD_API_METHOD_2(setAttribute);
	ADD_API_METHOD_1(setBypassed);
    ADD_API_METHOD_1(getAttribute);
	ADD_API_METHOD_1(getCurrentLevel);
	ADD_API_METHOD_0(exportState);
	ADD_API_METHOD_1(restoreState);
	ADD_API_METHOD_1(restoreScriptControls);
	ADD_API_METHOD_0(exportScriptControls);
	ADD_API_METHOD_0(getNumAttributes);
	ADD_API_METHOD_3(addModulator);
    
	ADD_API_METHOD_1(getModulatorChain);
	ADD_API_METHOD_3(addGlobalModulator);
	ADD_API_METHOD_3(addStaticGlobalModulator);
};


void ScriptingObjects::ScriptingEffect::rightClickCallback(const MouseEvent& e, Component* t)
{
	Helpers::showProcessorEditorPopup(e, t, effect.get());
}

juce::String ScriptingObjects::ScriptingEffect::getId() const
{
	if (checkValidObject())
		return effect->getId();

	return String();
}

void ScriptingObjects::ScriptingEffect::setAttribute(int parameterIndex, float newValue)
{
	if (checkValidObject())
	{
		effect->setAttribute(parameterIndex, newValue, sendNotification);
	}
}

float ScriptingObjects::ScriptingEffect::getAttribute(int parameterIndex)
{
    if (checkValidObject())
    {
        return effect->getAttribute(parameterIndex);
    }

	return 0.0f;
}

int ScriptingObjects::ScriptingEffect::getNumAttributes() const
{
	if (checkValidObject())
	{
		return effect->getNumParameters();
	}

	return 0;
}

void ScriptingObjects::ScriptingEffect::setBypassed(bool shouldBeBypassed)
{
	if (checkValidObject())
	{
		effect->setBypassed(shouldBeBypassed, sendNotification);
		effect->sendChangeMessage();
	}
}



String ScriptingObjects::ScriptingEffect::exportState()
{
	if (checkValidObject())
	{
		return ProcessorHelpers::getBase64String(effect);
	}

	return String();
}

void ScriptingObjects::ScriptingEffect::restoreState(String base64State)
{
	if (checkValidObject())
	{
		SuspendHelpers::ScopedTicket ticket(effect->getMainController());

		effect->getMainController()->getJavascriptThreadPool().killVoicesAndExtendTimeOut(dynamic_cast<JavascriptProcessor*>(getScriptProcessor()));

		LockHelpers::freeToGo(effect->getMainController());

		ProcessorHelpers::restoreFromBase64String(effect, base64State);

	}
}

String ScriptingObjects::ScriptingEffect::exportScriptControls()
{
	if (dynamic_cast<ProcessorWithScriptingContent*>(effect.get()) == nullptr)
	{
		reportScriptError("exportScriptControls can only be used on Script Processors");
	}

	if (checkValidObject())
	{
		return ProcessorHelpers::getBase64String(effect, false, true);
	}

	return String();
}

void ScriptingObjects::ScriptingEffect::restoreScriptControls(String base64Controls)
{
	if (dynamic_cast<ProcessorWithScriptingContent*>(effect.get()) == nullptr)
	{
		reportScriptError("restoreScriptControls can only be used on Script Processors");
	}

	if (checkValidObject())
	{
		ProcessorHelpers::restoreFromBase64String(effect, base64Controls, true);
	}
}

float ScriptingObjects::ScriptingEffect::getCurrentLevel(bool leftChannel)
{
	if (checkValidObject())
	{
		return leftChannel ? effect->getDisplayValues().outL : effect->getDisplayValues().outR;
	}

	return 0.0f;
}

var ScriptingObjects::ScriptingEffect::addModulator(var chainIndex, var typeName, var modName)
{
	if (checkValidObject())
	{
		ModulatorChain *c = dynamic_cast<ModulatorChain*>(effect->getChildProcessor(chainIndex));

		if (c == nullptr)
			reportScriptError("Modulator Chain with index " + chainIndex.toString() + " does not exist");

		Processor* p = moduleHandler.addModule(c, typeName, modName, -1);

		if (p != nullptr)
		{
			auto mod = new ScriptingObjects::ScriptingModulator(getScriptProcessor(), dynamic_cast<Modulator*>(p));
			return var(mod);
		}

		return var();
	}
	else
	{
		return var();
	}
	
}

var ScriptingObjects::ScriptingEffect::getModulatorChain(var chainIndex)
{
	if (checkValidObject())
	{
		auto c = dynamic_cast<Modulator*>(effect->getChildProcessor(chainIndex));

		if (c == nullptr)
			reportScriptError("Modulator Chain with index " + chainIndex.toString() + " does not exist");

		auto mod = new ScriptingModulator(getScriptProcessor(), c);

		return var(mod);
	}
	else
	{
		return var();
	}
}

var ScriptingObjects::ScriptingEffect::addGlobalModulator(var chainIndex, var globalMod, String modName)
{
	if (checkValidObject())
	{
		if (auto gm = dynamic_cast<ScriptingModulator*>(globalMod.getObject()))
		{
			ModulatorChain *c = dynamic_cast<ModulatorChain*>(effect->getChildProcessor(chainIndex));

			if (c == nullptr)
				reportScriptError("Modulator Chain with index " + chainIndex.toString() + " does not exist");

			auto p = moduleHandler.addAndConnectToGlobalModulator(c, gm->getModulator(), modName);

			if (p != nullptr)
			{
				auto mod = new ScriptingObjects::ScriptingModulator(getScriptProcessor(), p);
				return var(mod);
			}

			return var();
		}
	}

	return var();
}

var ScriptingObjects::ScriptingEffect::addStaticGlobalModulator(var chainIndex, var timeVariantMod, String modName)
{
	if (checkValidObject())
	{
		if (auto gm = dynamic_cast<ScriptingModulator*>(timeVariantMod.getObject()))
		{
			ModulatorChain *c = dynamic_cast<ModulatorChain*>(effect->getChildProcessor(chainIndex));

			if (c == nullptr)
				reportScriptError("Modulator Chain with index " + chainIndex.toString() + " does not exist");

			auto p = moduleHandler.addAndConnectToGlobalModulator(c, gm->getModulator(), modName, true);

			if (p != nullptr)
			{
				auto mod = new ScriptingObjects::ScriptingModulator(getScriptProcessor(), p);
				return var(mod);
			}

			return var();
		}
	}

	return var();
}

ScriptingObjects::ScriptingEffect::FilterModeObject::FilterModeObject(const ProcessorWithScriptingContent* p) :
	ConstScriptingObject(const_cast<ProcessorWithScriptingContent*>(p), (int)FilterBank::FilterMode::numFilterModes)
{
	

#define ADD_FILTER_CONSTANT(x) addConstant(#x, (int)FilterBank::FilterMode::x)

	ADD_FILTER_CONSTANT(LowPass);
	ADD_FILTER_CONSTANT(HighPass);
	ADD_FILTER_CONSTANT(LowShelf);
	ADD_FILTER_CONSTANT(HighShelf);
	ADD_FILTER_CONSTANT(Peak);
	ADD_FILTER_CONSTANT(ResoLow);
	ADD_FILTER_CONSTANT(StateVariableLP);
	ADD_FILTER_CONSTANT(StateVariableHP);
	ADD_FILTER_CONSTANT(MoogLP);
	ADD_FILTER_CONSTANT(OnePoleLowPass);
	ADD_FILTER_CONSTANT(OnePoleHighPass);
	ADD_FILTER_CONSTANT(StateVariablePeak);
	ADD_FILTER_CONSTANT(StateVariableNotch);
	ADD_FILTER_CONSTANT(StateVariableBandPass);
	ADD_FILTER_CONSTANT(Allpass);
	ADD_FILTER_CONSTANT(LadderFourPoleLP);
	ADD_FILTER_CONSTANT(LadderFourPoleHP);
	ADD_FILTER_CONSTANT(RingMod);

#undef ADD_FILTER_CONSTANT
}



// ScriptingSlotFX ==============================================================================================================


struct ScriptingObjects::ScriptingSlotFX::Wrapper
{
    API_METHOD_WRAPPER_1(ScriptingSlotFX, setEffect);
    API_VOID_METHOD_WRAPPER_0(ScriptingSlotFX, clear);
	API_VOID_METHOD_WRAPPER_1(ScriptingSlotFX, swap);
    
};

ScriptingObjects::ScriptingSlotFX::ScriptingSlotFX(ProcessorWithScriptingContent *p, EffectProcessor *fx) :
ConstScriptingObject(p, fx != nullptr ? fx->getNumParameters()+1 : 1),
slotFX(fx)
{
    if (fx != nullptr)
    {
        setName(fx->getId());
        
        addScriptParameters(this, slotFX.get());
        
        for (int i = 0; i < fx->getNumParameters(); i++)
        {
            addConstant(fx->getIdentifierForParameterIndex(i).toString(), var(i));
        }
    }
    else
    {
        setName("Invalid Effect");
    }
    
    ADD_API_METHOD_1(setEffect);
    ADD_API_METHOD_0(clear);
	ADD_API_METHOD_1(swap);
    
};



void ScriptingObjects::ScriptingSlotFX::clear()
{
	if (auto slot = getSlotFX())
	{
        slot->reset();
	}
	else
	{
		reportScriptError("Invalid Slot");
	}
}


ScriptingObjects::ScriptingEffect* ScriptingObjects::ScriptingSlotFX::setEffect(String effectName)
{
	if(auto slot = getSlotFX())
    {
		auto jp = dynamic_cast<JavascriptProcessor*>(getScriptProcessor());

		{
			SuspendHelpers::ScopedTicket ticket(slot->getMainController());

			slot->getMainController()->getJavascriptThreadPool().killVoicesAndExtendTimeOut(jp);

			LockHelpers::freeToGo(slot->getMainController());
			slot->setEffect(effectName);
		}

		jassert(slot->getCurrentEffect()->getType() == Identifier(effectName));

		return new ScriptingEffect(getScriptProcessor(), slot->getCurrentEffect());
    }
	else
	{
		reportScriptError("Invalid Slot");
		RETURN_IF_NO_THROW(new ScriptingEffect(getScriptProcessor(), nullptr))
	}
}

void ScriptingObjects::ScriptingSlotFX::swap(var otherSlot)
{
	if (auto t = getSlotFX())
	{
		if (auto sl = dynamic_cast<ScriptingSlotFX*>(otherSlot.getObject()))
		{
			if (auto other = sl->getSlotFX())
			{
				t->swap(other);
			}
			else
			{
				reportScriptError("Target Slot is invalid");
			}
		}
		else
		{
			reportScriptError("Target Slot does not exist");
		}
	}
	else
	{
		reportScriptError("Source Slot is invalid");
	}
}

SlotFX* ScriptingObjects::ScriptingSlotFX::getSlotFX()
{
	return dynamic_cast<SlotFX*>(slotFX.get());
}

struct ScriptingObjects::ScriptRoutingMatrix::Wrapper
{
	API_METHOD_WRAPPER_2(ScriptRoutingMatrix, addConnection);
	API_METHOD_WRAPPER_2(ScriptRoutingMatrix, removeConnection);
	API_VOID_METHOD_WRAPPER_0(ScriptRoutingMatrix, clear);
	API_METHOD_WRAPPER_1(ScriptRoutingMatrix, getSourceGainValue);
};

ScriptingObjects::ScriptRoutingMatrix::ScriptRoutingMatrix(ProcessorWithScriptingContent *p, Processor *processor):
	ConstScriptingObject(p, 2),
	rp(processor)
{
	ADD_API_METHOD_2(addConnection);
	ADD_API_METHOD_2(removeConnection);
	ADD_API_METHOD_0(clear);
	ADD_API_METHOD_1(getSourceGainValue);

	if (auto r = dynamic_cast<RoutableProcessor*>(rp.get()))
	{
		addConstant("NumInputs", r->getMatrix().getNumSourceChannels());
		addConstant("NumOutputs", r->getMatrix().getNumDestinationChannels());
	}
	else
	{
		jassertfalse;
		addConstant("NumInputs", -1);
		addConstant("NumOutputs", -1);
	}
}

bool ScriptingObjects::ScriptRoutingMatrix::addConnection(int sourceIndex, int destinationIndex)
{
	if (checkValidObject())
	{
		if (auto r = dynamic_cast<RoutableProcessor*>(rp.get()))
		{
			return r->getMatrix().addConnection(sourceIndex, destinationIndex);
		}
		else
			return false;
	}

	return false;
}

bool ScriptingObjects::ScriptRoutingMatrix::removeConnection(int sourceIndex, int destinationIndex)
{
	if (checkValidObject())
	{
		if (auto r = dynamic_cast<RoutableProcessor*>(rp.get()))
		{
			return r->getMatrix().removeConnection(sourceIndex, destinationIndex);
		}
		else
			return false;
	}

	return false;
}

void ScriptingObjects::ScriptRoutingMatrix::clear()
{
	if (checkValidObject())
	{
		if (auto r = dynamic_cast<RoutableProcessor*>(rp.get()))
		{
			r->getMatrix().resetToDefault();
			r->getMatrix().removeConnection(0, 0);
			r->getMatrix().removeConnection(1, 1);
		}
		
	}
}

float ScriptingObjects::ScriptRoutingMatrix::getSourceGainValue(int channelIndex)
{
	if (checkValidObject())
	{
		if (auto r = dynamic_cast<RoutableProcessor*>(rp.get()))
		{
			if (isPositiveAndBelow(channelIndex, r->getMatrix().getNumSourceChannels()))
			{
				return r->getMatrix().getGainValue(channelIndex, true);
			}
		}

	}

	return 0.0f;
}

// ScriptingSynth ==============================================================================================================

struct ScriptingObjects::ScriptingSynth::Wrapper
{
	API_VOID_METHOD_WRAPPER_2(ScriptingSynth, setAttribute);
    API_METHOD_WRAPPER_1(ScriptingSynth, getAttribute);
	API_METHOD_WRAPPER_0(ScriptingSynth, getNumAttributes);
	API_VOID_METHOD_WRAPPER_1(ScriptingSynth, setBypassed);
	API_METHOD_WRAPPER_1(ScriptingSynth, getChildSynthByIndex);
	API_METHOD_WRAPPER_0(ScriptingSynth, exportState);
	API_METHOD_WRAPPER_1(ScriptingSynth, getCurrentLevel);
	API_VOID_METHOD_WRAPPER_1(ScriptingSynth, restoreState);
	API_METHOD_WRAPPER_3(ScriptingSynth, addModulator);
	API_METHOD_WRAPPER_1(ScriptingSynth, getModulatorChain);
	API_METHOD_WRAPPER_3(ScriptingSynth, addGlobalModulator);
	API_METHOD_WRAPPER_3(ScriptingSynth, addStaticGlobalModulator);
	API_METHOD_WRAPPER_0(ScriptingSynth, asSampler);
	API_METHOD_WRAPPER_0(ScriptingSynth, getRoutingMatrix);
	API_METHOD_WRAPPER_0(ScriptingSynth, getId);
};

ScriptingObjects::ScriptingSynth::ScriptingSynth(ProcessorWithScriptingContent *p, ModulatorSynth *synth_) :
	ConstScriptingObject(p, synth_ != nullptr ? synth_->getNumParameters() + 1 : 1),
	synth(synth_),
	moduleHandler(synth_, dynamic_cast<JavascriptProcessor*>(p))
{
	if (synth != nullptr)
	{
		setName(synth->getId());

		addScriptParameters(this, synth.get());

		for (int i = 0; i < synth->getNumParameters(); i++)
		{
			addConstant(synth->getIdentifierForParameterIndex(i).toString(), var(i));
		}
	}
	else
	{
		setName("Invalid Effect");
	}

	ADD_API_METHOD_0(getId);
	ADD_API_METHOD_2(setAttribute);
    ADD_API_METHOD_1(getAttribute);
	ADD_API_METHOD_1(setBypassed);
	ADD_API_METHOD_1(getChildSynthByIndex);
	ADD_API_METHOD_1(getCurrentLevel);
	ADD_API_METHOD_0(exportState);
	ADD_API_METHOD_1(restoreState);
	ADD_API_METHOD_0(getNumAttributes);
	ADD_API_METHOD_3(addModulator);
	ADD_API_METHOD_1(getModulatorChain);
	ADD_API_METHOD_3(addGlobalModulator);
	ADD_API_METHOD_3(addStaticGlobalModulator);
	ADD_API_METHOD_0(asSampler);
	ADD_API_METHOD_0(getRoutingMatrix);
};


void ScriptingObjects::ScriptingSynth::rightClickCallback(const MouseEvent& e, Component* t)
{
	Helpers::showProcessorEditorPopup(e, t, synth);
}

String ScriptingObjects::ScriptingSynth::getId() const
{
	if (checkValidObject())
		return synth->getId();

	return String();
}

void ScriptingObjects::ScriptingSynth::setAttribute(int parameterIndex, float newValue)
{
	if (checkValidObject())
	{
		synth->setAttribute(parameterIndex, newValue, sendNotification);
	}
}

float ScriptingObjects::ScriptingSynth::getAttribute(int parameterIndex)
{
    if (checkValidObject())
    {
        return synth->getAttribute(parameterIndex);
    }

	return 0.0f;
}

int ScriptingObjects::ScriptingSynth::getNumAttributes() const
{
	if (checkValidObject())
	{
		return synth->getNumParameters();
	}

	return 0;
}

void ScriptingObjects::ScriptingSynth::setBypassed(bool shouldBeBypassed)
{
	if (checkValidObject())
	{
		synth->setBypassed(shouldBeBypassed, sendNotification);
		synth->sendChangeMessage();
	}
}

ScriptingObjects::ScriptingSynth* ScriptingObjects::ScriptingSynth::getChildSynthByIndex(int index)
{
	if (getScriptProcessor()->objectsCanBeCreated())
	{
		if (Chain* c = dynamic_cast<Chain*>(synth.get()))
		{
			if (index >= 0 && index < c->getHandler()->getNumProcessors())
			{
				return new ScriptingObjects::ScriptingSynth(getScriptProcessor(), dynamic_cast<ModulatorSynth*>(c->getHandler()->getProcessor(index)));
			}
		}

		return new ScriptingObjects::ScriptingSynth(getScriptProcessor(), nullptr);
	}
	else
	{
		reportIllegalCall("getChildSynth()", "onInit");
		RETURN_IF_NO_THROW(new ScriptingObjects::ScriptingSynth(getScriptProcessor(), nullptr))
	}
}

String ScriptingObjects::ScriptingSynth::exportState()
{
	if (checkValidObject())
	{
		return ProcessorHelpers::getBase64String(synth);
	}

	return String();
}

void ScriptingObjects::ScriptingSynth::restoreState(String base64State)
{
	if (checkValidObject())
	{
		ProcessorHelpers::restoreFromBase64String(synth, base64State);
	}
}

float ScriptingObjects::ScriptingSynth::getCurrentLevel(bool leftChannel)
{
	if (checkValidObject())
	{
		return leftChannel ? synth->getDisplayValues().outL : synth->getDisplayValues().outR;
	}

	return 0.0f;
}

var ScriptingObjects::ScriptingSynth::addModulator(var chainIndex, var typeName, var modName)
{
	if (checkValidObject())
	{
		ModulatorChain *c = dynamic_cast<ModulatorChain*>(synth->getChildProcessor(chainIndex));

		if (c == nullptr)
			reportScriptError("Modulator Chain with index " + chainIndex.toString() + " does not exist");

		Processor* p = moduleHandler.addModule(c, typeName, modName, -1);

		if (p != nullptr)
		{
			auto mod = new ScriptingObjects::ScriptingModulator(getScriptProcessor(), dynamic_cast<Modulator*>(p));
			return var(mod);
		}
	}

	return var();
}

var ScriptingObjects::ScriptingSynth::getModulatorChain(var chainIndex)
{
	if (checkValidObject())
	{
		auto c = dynamic_cast<Modulator*>(synth->getChildProcessor(chainIndex));

		if (c == nullptr)
			reportScriptError("Modulator Chain with index " + chainIndex.toString() + " does not exist");

		auto mod = new ScriptingModulator(getScriptProcessor(), c);

		return var(mod);
	}
	else
	{
		return var();
	}
}

var ScriptingObjects::ScriptingSynth::addGlobalModulator(var chainIndex, var globalMod, String modName)
{
	if (checkValidObject())
	{
		if (auto gm = dynamic_cast<ScriptingModulator*>(globalMod.getObject()))
		{
			ModulatorChain *c = dynamic_cast<ModulatorChain*>(synth->getChildProcessor(chainIndex));

			if (c == nullptr)
				reportScriptError("Modulator Chain with index " + chainIndex.toString() + " does not exist");

			auto p = moduleHandler.addAndConnectToGlobalModulator(c, gm->getModulator(), modName);

			if (p != nullptr)
			{
				auto mod = new ScriptingObjects::ScriptingModulator(getScriptProcessor(), p);
				return var(mod);
			}
		}
	}

	return var();
}

var ScriptingObjects::ScriptingSynth::addStaticGlobalModulator(var chainIndex, var timeVariantMod, String modName)
{
	if (checkValidObject())
	{
		if (auto gm = dynamic_cast<ScriptingModulator*>(timeVariantMod.getObject()))
		{
			ModulatorChain *c = dynamic_cast<ModulatorChain*>(synth->getChildProcessor(chainIndex));

			if (c == nullptr)
				reportScriptError("Modulator Chain with index " + chainIndex.toString() + " does not exist");

			auto p = moduleHandler.addAndConnectToGlobalModulator(c, gm->getModulator(), modName, true);

			if (p != nullptr)
			{
				auto mod = new ScriptingObjects::ScriptingModulator(getScriptProcessor(), p);
				return var(mod);
			}
		}
	}

	return var();
}

var ScriptingObjects::ScriptingSynth::asSampler()
{
	if (checkValidObject())
	{
		auto sampler = dynamic_cast<ModulatorSampler*>(synth.get());

		if (sampler == nullptr)
			return var(); // don't complain here, handle it on scripting level

		auto t = new ScriptingApi::Sampler(getScriptProcessor(), sampler);
		return var(t);
	}

	auto t = new ScriptingApi::Sampler(getScriptProcessor(), nullptr);
	return var(t);
}

var ScriptingObjects::ScriptingSynth::getRoutingMatrix()
{
	auto r = new ScriptRoutingMatrix(getScriptProcessor(), synth.get());
	return var(r);
}

// ScriptingMidiProcessor ==============================================================================================================

struct ScriptingObjects::ScriptingMidiProcessor::Wrapper
{
	API_VOID_METHOD_WRAPPER_2(ScriptingMidiProcessor, setAttribute);
    API_METHOD_WRAPPER_1(ScriptingMidiProcessor, getAttribute);
	API_METHOD_WRAPPER_0(ScriptingMidiProcessor, getNumAttributes);
	API_VOID_METHOD_WRAPPER_1(ScriptingMidiProcessor, setBypassed);
	API_METHOD_WRAPPER_0(ScriptingMidiProcessor, exportState);
	API_VOID_METHOD_WRAPPER_1(ScriptingMidiProcessor, restoreState);
	API_VOID_METHOD_WRAPPER_1(ScriptingMidiProcessor, restoreScriptControls);
	API_METHOD_WRAPPER_0(ScriptingMidiProcessor, exportScriptControls);
	API_METHOD_WRAPPER_0(ScriptingMidiProcessor, getId);
	
};

ScriptingObjects::ScriptingMidiProcessor::ScriptingMidiProcessor(ProcessorWithScriptingContent *p, MidiProcessor *mp_) :
ConstScriptingObject(p, mp_ != nullptr ? mp_->getNumParameters()+1 : 1),
mp(mp_)
{
	if (mp != nullptr)
	{
		setName(mp->getId());

		addScriptParameters(this, mp.get());

		for (int i = 0; i < mp->getNumParameters(); i++)
		{
			addConstant(mp->getIdentifierForParameterIndex(i).toString(), var(i));
		}
	}
	else
	{
		setName("Invalid MidiProcessor");
	}

	ADD_API_METHOD_2(setAttribute);
    ADD_API_METHOD_1(getAttribute);
	ADD_API_METHOD_1(setBypassed);
	ADD_API_METHOD_0(exportState);
	ADD_API_METHOD_1(restoreState);
	ADD_API_METHOD_0(getId);
	ADD_API_METHOD_1(restoreScriptControls);
	ADD_API_METHOD_0(exportScriptControls);
	ADD_API_METHOD_0(getNumAttributes);
}

void ScriptingObjects::ScriptingMidiProcessor::rightClickCallback(const MouseEvent& e, Component* t)
{
	Helpers::showProcessorEditorPopup(e, t, mp);
}

int ScriptingObjects::ScriptingMidiProcessor::getCachedIndex(const var &indexExpression) const
{
	if (checkValidObject())
	{
		Identifier id(indexExpression.toString());

		for (int i = 0; i < mp->getNumParameters(); i++)
		{
			if (id == mp->getIdentifierForParameterIndex(i)) return i;
		}
	}

	return -1;
}

void ScriptingObjects::ScriptingMidiProcessor::assign(const int index, var newValue)
{
	setAttribute(index, (float)newValue);
}

var ScriptingObjects::ScriptingMidiProcessor::getAssignedValue(int /*index*/) const
{
	return 1.0; // Todo...
}

String ScriptingObjects::ScriptingMidiProcessor::getId() const
{
	if (checkValidObject())
		return mp->getId();

	return String();
}

void ScriptingObjects::ScriptingMidiProcessor::setAttribute(int index, float value)
{
	if (checkValidObject())
	{
		mp->setAttribute(index, value, sendNotification);
	}
}

float ScriptingObjects::ScriptingMidiProcessor::getAttribute(int parameterIndex)
{
    if (checkValidObject())
    {
        return mp->getAttribute(parameterIndex);
    }

	return 0.0f;
}

int ScriptingObjects::ScriptingMidiProcessor::getNumAttributes() const
{
	if (checkValidObject())
	{
		return mp->getNumParameters();
	}

	return 0;
}

void ScriptingObjects::ScriptingMidiProcessor::setBypassed(bool shouldBeBypassed)
{
	if (checkValidObject())
	{
		mp->setBypassed(shouldBeBypassed, sendNotification);
		mp->sendChangeMessage();
	}
}

String ScriptingObjects::ScriptingMidiProcessor::exportState()
{
	if (checkValidObject())
	{
		return ProcessorHelpers::getBase64String(mp, false, false);
	}

	return String();
}

void ScriptingObjects::ScriptingMidiProcessor::restoreState(String base64State)
{
	if (checkValidObject())
	{
		ProcessorHelpers::restoreFromBase64String(mp, base64State, false);
	}
}

String ScriptingObjects::ScriptingMidiProcessor::exportScriptControls()
{
	if (dynamic_cast<ProcessorWithScriptingContent*>(mp.get()) == nullptr)
	{
		reportScriptError("exportScriptControls can only be used on Script Processors");
	}

	if (checkValidObject())
	{
		return ProcessorHelpers::getBase64String(mp, false, true);
	}

	return String();
}

void ScriptingObjects::ScriptingMidiProcessor::restoreScriptControls(String base64Controls)
{
	if (dynamic_cast<ProcessorWithScriptingContent*>(mp.get()) == nullptr)
	{
		reportScriptError("restoreScriptControls can only be used on Script Processors");
	}

	if (checkValidObject())
	{
		ProcessorHelpers::restoreFromBase64String(mp, base64Controls, true);
	}
}

// ScriptingAudioSampleProcessor ==============================================================================================================

struct ScriptingObjects::ScriptingAudioSampleProcessor::Wrapper
{
	API_VOID_METHOD_WRAPPER_2(ScriptingAudioSampleProcessor, setAttribute);
    API_METHOD_WRAPPER_1(ScriptingAudioSampleProcessor, getAttribute);
	API_METHOD_WRAPPER_0(ScriptingAudioSampleProcessor, getNumAttributes);
	API_VOID_METHOD_WRAPPER_1(ScriptingAudioSampleProcessor, setBypassed);
	API_METHOD_WRAPPER_0(ScriptingAudioSampleProcessor, getSampleLength);
	API_VOID_METHOD_WRAPPER_2(ScriptingAudioSampleProcessor, setSampleRange);
	API_VOID_METHOD_WRAPPER_1(ScriptingAudioSampleProcessor, setFile);
};


ScriptingObjects::ScriptingAudioSampleProcessor::ScriptingAudioSampleProcessor(ProcessorWithScriptingContent *p, AudioSampleProcessor *sampleProcessor) :
ConstScriptingObject(p, dynamic_cast<Processor*>(sampleProcessor) != nullptr ? dynamic_cast<Processor*>(sampleProcessor)->getNumParameters() : 0),
audioSampleProcessor(dynamic_cast<Processor*>(sampleProcessor))
{
	if (audioSampleProcessor != nullptr)
	{
		setName(audioSampleProcessor->getId());

		for (int i = 0; i < audioSampleProcessor->getNumParameters(); i++)
		{
			addConstant(audioSampleProcessor->getIdentifierForParameterIndex(i).toString(), var(i));
		}
	}
	else
	{
		setName("Invalid Processor");
	}

	ADD_API_METHOD_2(setAttribute);
    ADD_API_METHOD_1(getAttribute);
	ADD_API_METHOD_0(getNumAttributes);
	ADD_API_METHOD_1(setBypassed);
	ADD_API_METHOD_0(getSampleLength);
	ADD_API_METHOD_2(setSampleRange);
	ADD_API_METHOD_1(setFile);
}



void ScriptingObjects::ScriptingAudioSampleProcessor::setAttribute(int parameterIndex, float newValue)
{
	if (checkValidObject())
	{
		audioSampleProcessor->setAttribute(parameterIndex, newValue, sendNotification);
	}
}

float ScriptingObjects::ScriptingAudioSampleProcessor::getAttribute(int parameterIndex)
{
    if (checkValidObject())
    {
        return audioSampleProcessor->getAttribute(parameterIndex);
    }

	return 0.0f;
}

int ScriptingObjects::ScriptingAudioSampleProcessor::getNumAttributes() const
{
	if (checkValidObject())
	{
		return audioSampleProcessor->getNumParameters();
	}

	return 0;
}

void ScriptingObjects::ScriptingAudioSampleProcessor::setBypassed(bool shouldBeBypassed)
{
	if (checkValidObject())
	{
		audioSampleProcessor->setBypassed(shouldBeBypassed, sendNotification);
		audioSampleProcessor->sendChangeMessage();
	}
}


void ScriptingObjects::ScriptingAudioSampleProcessor::setFile(String fileName)
{
	if (checkValidObject())
	{
		auto asp = dynamic_cast<AudioSampleProcessor*>(audioSampleProcessor.get());

		ScopedLock sl(asp->getFileLock());

		asp->setLoadedFile(fileName, true);
	}
}

void ScriptingObjects::ScriptingAudioSampleProcessor::setSampleRange(int start, int end)
{
	if (checkValidObject())
	{
		ScopedLock sl(audioSampleProcessor->getMainController()->getLock());
		dynamic_cast<AudioSampleProcessor*>(audioSampleProcessor.get())->setRange(Range<int>(start, end));

	}
}

int ScriptingObjects::ScriptingAudioSampleProcessor::getSampleLength() const
{
	if (checkValidObject())
	{
		return dynamic_cast<const AudioSampleProcessor*>(audioSampleProcessor.get())->getTotalLength();
	}
	else return 0;
}

// ScriptingTableProcessor ==============================================================================================================

struct ScriptingObjects::ScriptingTableProcessor::Wrapper
{
	API_VOID_METHOD_WRAPPER_3(ScriptingTableProcessor, addTablePoint);
	API_VOID_METHOD_WRAPPER_1(ScriptingTableProcessor, reset);
	API_VOID_METHOD_WRAPPER_5(ScriptingTableProcessor, setTablePoint);
};



ScriptingObjects::ScriptingTableProcessor::ScriptingTableProcessor(ProcessorWithScriptingContent *p, LookupTableProcessor *tableProcessor_) :
ConstScriptingObject(p, dynamic_cast<Processor*>(tableProcessor_) != nullptr ? dynamic_cast<Processor*>(tableProcessor_)->getNumParameters() : 0),
tableProcessor(dynamic_cast<Processor*>(tableProcessor_))
{
	if (tableProcessor != nullptr)
	{
		setName(tableProcessor->getId());

		for (int i = 0; i < tableProcessor->getNumParameters(); i++)
		{
			addConstant(tableProcessor->getIdentifierForParameterIndex(i).toString(), var(i));
		}
	}
	else
	{
		setName("Invalid Processor");
	}

	ADD_API_METHOD_3(addTablePoint);
	ADD_API_METHOD_1(reset);
	ADD_API_METHOD_5(setTablePoint);
}



void ScriptingObjects::ScriptingTableProcessor::setTablePoint(int tableIndex, int pointIndex, float x, float y, float curve)
{
	if (tableProcessor != nullptr)
	{
		Table *table = dynamic_cast<LookupTableProcessor*>(tableProcessor.get())->getTable(tableIndex);

		if (table != nullptr)
		{
			table->setTablePoint(pointIndex, x, y, curve);
			table->sendChangeMessage();
		}
	}
}


void ScriptingObjects::ScriptingTableProcessor::addTablePoint(int tableIndex, float x, float y)
{
	if (tableProcessor != nullptr)
	{
		Table *table = dynamic_cast<LookupTableProcessor*>(tableProcessor.get())->getTable(tableIndex);

		if (table != nullptr)
		{
			table->addTablePoint(x, y);
			table->sendChangeMessage();
		}
	}
}


void ScriptingObjects::ScriptingTableProcessor::reset(int tableIndex)
{
	if (tableProcessor != nullptr)
	{
		Table *table = dynamic_cast<LookupTableProcessor*>(tableProcessor.get())->getTable(tableIndex);

		if (table != nullptr)
		{
			table->reset();
			table->sendChangeMessage();
		}
	}
}

// TimerObject ==============================================================================================================

struct ScriptingObjects::TimerObject::Wrapper
{
	DYNAMIC_METHOD_WRAPPER(TimerObject, startTimer, (int)ARG(0));
	DYNAMIC_METHOD_WRAPPER(TimerObject, stopTimer);
	DYNAMIC_METHOD_WRAPPER(TimerObject, setTimerCallback, ARG(0));
};

ScriptingObjects::TimerObject::TimerObject(ProcessorWithScriptingContent *p) :
	DynamicScriptingObject(p),
	ControlledObject(p->getMainController_(), true),
	it(this)
{
	ADD_DYNAMIC_METHOD(startTimer);
	ADD_DYNAMIC_METHOD(stopTimer);
	ADD_DYNAMIC_METHOD(setTimerCallback);
}


ScriptingObjects::TimerObject::~TimerObject()
{
	it.stopTimer();
	removeProperty("callback");
}

void ScriptingObjects::TimerObject::timerCallback()
{
	auto callback = getProperty("callback");

	if (HiseJavascriptEngine::isJavascriptFunction(callback))
	{
        WeakReference<TimerObject> ref(this);
        
		auto f = [ref, callback](JavascriptProcessor* )
		{
            Result r = Result::ok();
            
            if(ref != nullptr)
            {
                
                ref.get()->timerCallbackInternal(callback, r);
            }
			
			return r;
		};

		auto mc = getScriptProcessor()->getMainController_();
		mc->getJavascriptThreadPool().addJob(JavascriptThreadPool::Task::LowPriorityCallbackExecution, 
											 dynamic_cast<JavascriptProcessor*>(getScriptProcessor()), 
											 f);
	}
}

void ScriptingObjects::TimerObject::timerCallbackInternal(const var& callback, Result& r)
{
	jassert(LockHelpers::isLockedBySameThread(getScriptProcessor()->getMainController_(), LockHelpers::ScriptLock));

	var undefinedArgs;
	var thisObject(this);
	var::NativeFunctionArgs args(thisObject, &undefinedArgs, 0);

	auto engine = dynamic_cast<JavascriptMidiProcessor*>(getScriptProcessor())->getScriptEngine();

	jassert(engine != nullptr);

	if (engine != nullptr)
	{
		engine->maximumExecutionTime = RelativeTime(0.5);
		engine->callExternalFunction(callback, args, &r);

		if (r.failed())
		{
			stopTimer();
			debugError(getProcessor(), r.getErrorMessage());
		}
	}
	else
		stopTimer();
}

void ScriptingObjects::TimerObject::startTimer(int intervalInMilliSeconds)
{
	if (intervalInMilliSeconds > 10)
	{
		it.startTimer(intervalInMilliSeconds);
	}
	else
		throw String("Go easy on the timer");
}

void ScriptingObjects::TimerObject::stopTimer()
{
	it.stopTimer();
}

void ScriptingObjects::TimerObject::setTimerCallback(var callbackFunction)
{
	if (dynamic_cast<HiseJavascriptEngine::RootObject::FunctionObject*>(callbackFunction.getObject()))
	{
		setProperty("callback", callbackFunction);
	}
	else
		throw String("You need to pass in a function for the timer callback");
}

class PathPreviewComponent: public Component
{
public:

	PathPreviewComponent(Path &p_) : p(p_) { setSize(300, 300); }

	void paint(Graphics &g) override
	{
		g.setColour(Colours::white);
		p.scaleToFit(0.0f, 0.0f, (float)getWidth(), (float)getHeight(), true);
		g.fillPath(p);
	}

private:

	Path p;
};

void ScriptingObjects::PathObject::rightClickCallback(const MouseEvent &e, Component* componentToNotify)
{
#if USE_BACKEND

	auto *editor = GET_BACKEND_ROOT_WINDOW(componentToNotify);

	PathPreviewComponent* content = new PathPreviewComponent(p);
	
	MouseEvent ee = e.getEventRelativeTo(editor);

	editor->getRootFloatingTile()->showComponentInRootPopup(content, editor, ee.getMouseDownPosition());

#else

	ignoreUnused(e, componentToNotify);

#endif
}


struct ScriptingObjects::PathObject::Wrapper
{
	API_VOID_METHOD_WRAPPER_1(PathObject, loadFromData);
	API_VOID_METHOD_WRAPPER_0(PathObject, closeSubPath);
	API_VOID_METHOD_WRAPPER_2(PathObject, startNewSubPath);
	API_VOID_METHOD_WRAPPER_2(PathObject, lineTo);
	API_VOID_METHOD_WRAPPER_0(PathObject, clear);
	API_VOID_METHOD_WRAPPER_4(PathObject, quadraticTo);
	API_VOID_METHOD_WRAPPER_3(PathObject, addArc);
	API_METHOD_WRAPPER_1(PathObject, getBounds);
};

ScriptingObjects::PathObject::PathObject(ProcessorWithScriptingContent* p) :
ConstScriptingObject(p, 0)
{
	ADD_API_METHOD_1(loadFromData);
	ADD_API_METHOD_0(closeSubPath);
	ADD_API_METHOD_0(clear);
	ADD_API_METHOD_2(startNewSubPath);
	ADD_API_METHOD_2(lineTo);
	ADD_API_METHOD_4(quadraticTo);
	ADD_API_METHOD_3(addArc);
	ADD_API_METHOD_1(getBounds);
}

ScriptingObjects::PathObject::~PathObject()
{

}


void ScriptingObjects::PathObject::loadFromData(var data)
{
	if (data.isArray())
	{
		p.clear();

		Array<unsigned char> pathData;

		Array<var> *varData = data.getArray();

		const int numElements = varData->size();

		pathData.ensureStorageAllocated(numElements);

		for (int i = 0; i < numElements; i++)
		{
			pathData.add(static_cast<unsigned char>((int)varData->getUnchecked(i)));
		}

		p.loadPathFromData(pathData.getRawDataPointer(), numElements);
	}
}

void ScriptingObjects::PathObject::clear()
{
	p.clear();
}

void ScriptingObjects::PathObject::startNewSubPath(var x, var y)
{
    auto x_ = (float)x;
    auto y_ = (float)y;
    
	p.startNewSubPath(SANITIZED(x_), SANITIZED(y_));
}

void ScriptingObjects::PathObject::closeSubPath()
{
	p.closeSubPath();
}

void ScriptingObjects::PathObject::lineTo(var x, var y)
{
    auto x_ = (float)x;
    auto y_ = (float)y;
    
	p.lineTo(SANITIZED(x_), SANITIZED(y_));
}

void ScriptingObjects::PathObject::quadraticTo(var cx, var cy, var x, var y)
{
	p.quadraticTo(cx, cy, x, y);
}

void ScriptingObjects::PathObject::addArc(var area, var fromRadians, var toRadians)
{
	auto rect = ApiHelpers::getRectangleFromVar(area);

    auto fr = (float)fromRadians;
    auto tr = (float)toRadians;
    
	p.addArc(rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight(), SANITIZED(fr), SANITIZED(tr), true);
}

var ScriptingObjects::PathObject::getBounds(var scaleFactor)
{
	auto r = p.getBoundsTransformed(AffineTransform::scale(scaleFactor));

	Array<var> area;

	area.add(r.getX());
	area.add(r.getY());
	area.add(r.getWidth());
	area.add(r.getHeight());

	return var(area);
}

struct ScriptingObjects::GraphicsObject::Wrapper
{
	API_VOID_METHOD_WRAPPER_1(GraphicsObject, fillAll);
	API_VOID_METHOD_WRAPPER_1(GraphicsObject, setColour);
	API_VOID_METHOD_WRAPPER_1(GraphicsObject, setOpacity);
	API_VOID_METHOD_WRAPPER_1(GraphicsObject, fillRect);
	API_VOID_METHOD_WRAPPER_2(GraphicsObject, drawRect);
	API_VOID_METHOD_WRAPPER_3(GraphicsObject, drawRoundedRectangle);
	API_VOID_METHOD_WRAPPER_2(GraphicsObject, fillRoundedRectangle);
	API_VOID_METHOD_WRAPPER_5(GraphicsObject, drawLine);
	API_VOID_METHOD_WRAPPER_3(GraphicsObject, drawHorizontalLine);
	API_VOID_METHOD_WRAPPER_2(GraphicsObject, setFont);
	API_VOID_METHOD_WRAPPER_2(GraphicsObject, drawText);
	API_VOID_METHOD_WRAPPER_3(GraphicsObject, drawAlignedText);
	API_VOID_METHOD_WRAPPER_1(GraphicsObject, setGradientFill);
	API_VOID_METHOD_WRAPPER_2(GraphicsObject, drawEllipse);
	API_VOID_METHOD_WRAPPER_1(GraphicsObject, fillEllipse);
	API_VOID_METHOD_WRAPPER_4(GraphicsObject, drawImage);
	API_VOID_METHOD_WRAPPER_3(GraphicsObject, drawDropShadow);
	API_VOID_METHOD_WRAPPER_2(GraphicsObject, addDropShadowFromAlpha);
	API_VOID_METHOD_WRAPPER_3(GraphicsObject, drawTriangle);
	API_VOID_METHOD_WRAPPER_2(GraphicsObject, fillTriangle);
	API_VOID_METHOD_WRAPPER_2(GraphicsObject, fillPath);
	API_VOID_METHOD_WRAPPER_3(GraphicsObject, drawPath);
	API_VOID_METHOD_WRAPPER_2(GraphicsObject, rotate);
};

ScriptingObjects::GraphicsObject::GraphicsObject(ProcessorWithScriptingContent *p, ConstScriptingObject* parent_) :
ConstScriptingObject(p, 0),
parent(parent_),
rectangleResult(Result::ok())
{
	ADD_API_METHOD_1(fillAll);
	ADD_API_METHOD_1(setColour);
	ADD_API_METHOD_1(setOpacity);
	ADD_API_METHOD_2(drawRect);
	ADD_API_METHOD_1(fillRect);
	ADD_API_METHOD_3(drawRoundedRectangle);
	ADD_API_METHOD_2(fillRoundedRectangle);
	ADD_API_METHOD_5(drawLine);
	ADD_API_METHOD_3(drawHorizontalLine);
	ADD_API_METHOD_2(setFont);
	ADD_API_METHOD_2(drawText);
	ADD_API_METHOD_3(drawAlignedText);
	ADD_API_METHOD_1(setGradientFill);
	ADD_API_METHOD_2(drawEllipse);
	ADD_API_METHOD_1(fillEllipse);
	ADD_API_METHOD_4(drawImage);
	ADD_API_METHOD_3(drawDropShadow);
	ADD_API_METHOD_2(addDropShadowFromAlpha);
	ADD_API_METHOD_3(drawTriangle);
	ADD_API_METHOD_2(fillTriangle);
	ADD_API_METHOD_2(fillPath);
	ADD_API_METHOD_3(drawPath);
	ADD_API_METHOD_2(rotate);
}

ScriptingObjects::GraphicsObject::~GraphicsObject()
{
	parent = nullptr;
}

struct ScriptedDrawActions
{
	struct fillAll : public DrawActions::ActionBase
	{
		fillAll(Colour c_) : c(c_) {};
		void perform(Graphics& g) { g.fillAll(c); };
		Colour c;
	};

	struct setColour : public DrawActions::ActionBase
	{
		setColour(Colour c_) : c(c_) {};
		void perform(Graphics& g) { g.setColour(c); };
		Colour c;
	};

	struct addTransform : public DrawActions::ActionBase
	{
		addTransform(AffineTransform a_) : a(a_) {};
		void perform(Graphics& g) override { g.addTransform(a); };
		AffineTransform a;
	};

	struct fillPath : public DrawActions::ActionBase
	{
		fillPath(const Path& p_) : p(p_) {};
		void perform(Graphics& g) override { g.fillPath(p); };
		Path p;
	};

	struct drawPath : public DrawActions::ActionBase
	{
		drawPath(const Path& p_, float thickness_) : p(p_), thickness(thickness_) {};
		void perform(Graphics& g) override
		{
			PathStrokeType s(thickness);
			g.strokePath(p, s);
		}
		Path p;
		float thickness;
	};

	struct fillRect : public DrawActions::ActionBase
	{
		fillRect(Rectangle<float> area_) : area(area_) {};
		void perform(Graphics& g) { g.fillRect(area); };
		Rectangle<float> area;
	};

	struct fillEllipse : public DrawActions::ActionBase
	{
		fillEllipse(Rectangle<float> area_) : area(area_) {};
		void perform(Graphics& g) { g.fillEllipse(area); };
		Rectangle<float> area;
	};

	struct drawRect : public DrawActions::ActionBase
	{
		drawRect(Rectangle<float> area_, float borderSize_) : area(area_), borderSize(borderSize_) {};
		void perform(Graphics& g) { g.drawRect(area, borderSize); };
		Rectangle<float> area;
		float borderSize;
	};

	struct drawEllipse : public DrawActions::ActionBase
	{
		drawEllipse(Rectangle<float> area_, float borderSize_) : area(area_), borderSize(borderSize_) {};
		void perform(Graphics& g) { g.drawEllipse(area, borderSize); };
		Rectangle<float> area;
		float borderSize;
	};

	struct fillRoundedRect : public DrawActions::ActionBase
	{
		fillRoundedRect(Rectangle<float> area_, float cornerSize_) :
			area(area_), cornerSize(cornerSize_) {};
		void perform(Graphics& g) { g.fillRoundedRectangle(area, cornerSize); };
		Rectangle<float> area;
		float cornerSize;
	};

	struct drawRoundedRectangle : public DrawActions::ActionBase
	{
		drawRoundedRectangle(Rectangle<float> area_, float borderSize_, float cornerSize_) :
			area(area_), borderSize(borderSize_), cornerSize(cornerSize_) {};
		void perform(Graphics& g) { g.drawRoundedRectangle(area, cornerSize, borderSize); };
		Rectangle<float> area;
		float cornerSize, borderSize;
	};

	struct drawImageWithin : public DrawActions::ActionBase
	{
		drawImageWithin(const Image& img_, Rectangle<float> r_) :
			img(img_), r(r_){};

		void perform(Graphics& g) override
		{
			auto ri = r.toNearestInt();

			g.drawImageWithin(img, (int)r.getX(), (int)r.getY(), (int)r.getWidth(), (int)r.getHeight(), RectanglePlacement::centred);

			
			//			g.drawImage(img, ri.getX(), ri.getY(), (int)(r.getWidth() / scaleFactor), (int)(r.getHeight() / scaleFactor), 0, yOffset, (int)img.getWidth(), (int)((double)img.getHeight()));
		}

		Image img;
		Rectangle<float> r;
	};

	struct drawImage : public DrawActions::ActionBase
	{
		drawImage(const Image& img_, Rectangle<float> r_, float scaleFactor_, int yOffset_) :
			img(img_), r(r_), scaleFactor(scaleFactor_), yOffset(yOffset_) {};
		
		void perform(Graphics& g) override 
		{
			auto ri = r.toNearestInt();
			
			g.drawImage(img, (int)r.getX(), (int)r.getY(), (int)r.getWidth(), (int)r.getHeight(), 0, yOffset, (int)img.getWidth(), (int)((double)r.getHeight() * scaleFactor));


//			g.drawImage(img, ri.getX(), ri.getY(), (int)(r.getWidth() / scaleFactor), (int)(r.getHeight() / scaleFactor), 0, yOffset, (int)img.getWidth(), (int)((double)img.getHeight()));
		}

		Image img;
		Rectangle<float> r;
		float scaleFactor;
		int yOffset;
	};

	struct drawHorizontalLine : public DrawActions::ActionBase
	{
		drawHorizontalLine(int y_, float x1_, float x2_) :
			y(y_), x1(x1_), x2(x2_) {};
		void perform(Graphics& g) { g.drawHorizontalLine(y, x1, x2); };
		int y; float x1; float x2;
	};

	struct setOpacity : public DrawActions::ActionBase
	{
		setOpacity(float alpha_) :
			alpha(alpha_) {};
		void perform(Graphics& g) { g.setOpacity(alpha); };
		float alpha;
	};

	struct drawLine : public DrawActions::ActionBase
	{
		drawLine(float x1_, float x2_, float y1_, float y2_, float lineThickness_):
			x1(x1_), x2(x2_), y1(y1_), y2(y2_), lineThickness(lineThickness_) {};
		void perform(Graphics& g) { g.drawLine(x1, x2, y1, y2, lineThickness); };
		float x1, x2, y1, y2, lineThickness;
	};

	struct setFont : public DrawActions::ActionBase
	{
		setFont(Font f_) : f(f_) {};
		void perform(Graphics& g) { g.setFont(f); };
		Font f;
	};

	struct setGradientFill : public DrawActions::ActionBase
	{
		setGradientFill(ColourGradient grad_) : grad(grad_) {};
		void perform(Graphics& g) { g.setGradientFill(grad); };
		ColourGradient grad;
	};

	struct drawText : public DrawActions::ActionBase
	{
		drawText(const String& text_, Rectangle<float> area_, Justification j_=Justification::centred ) : text(text_), area(area_), j(j_) {};
		void perform(Graphics& g) override { g.drawText(text, area, j); };
		String text;
		Rectangle<float> area;
		Justification j;
	};

	struct drawDropShadow : public DrawActions::ActionBase
	{
		drawDropShadow(Rectangle<int> r_, DropShadow& shadow_) : r(r_), shadow(shadow_) {};
		void perform(Graphics& g) override { shadow.drawForRectangle(g, r); };
		Rectangle<int> r;
		DropShadow shadow;
	};

	struct addDropShadowFromAlpha : public DrawActions::ActionBase
	{
		addDropShadowFromAlpha(const DropShadow& shadow_) : shadow(shadow_) {};

		bool wantsCachedImage() const override { return true; };

		void perform(Graphics& g) override
		{
			shadow.drawForImage(g, cachedImage);
		}

		DropShadow shadow;
	};
};

void ScriptingObjects::GraphicsObject::fillAll(var colour)
{
	Colour c = ScriptingApi::Content::Helpers::getCleanedObjectColour(colour);
	drawActionHandler.addDrawAction(new ScriptedDrawActions::fillAll(c));
}

void ScriptingObjects::GraphicsObject::fillRect(var area)
{
	drawActionHandler.addDrawAction(new ScriptedDrawActions::fillRect(getRectangleFromVar(area)));
}

void ScriptingObjects::GraphicsObject::drawRect(var area, float borderSize)
{
	auto bs = (float)borderSize;
	drawActionHandler.addDrawAction(new ScriptedDrawActions::drawRect(getRectangleFromVar(area), SANITIZED(bs)));
}

void ScriptingObjects::GraphicsObject::fillRoundedRectangle(var area, float cornerSize)
{
    auto cs = (float)cornerSize;
	drawActionHandler.addDrawAction(new ScriptedDrawActions::fillRoundedRect(getRectangleFromVar(area), SANITIZED(cs)));
}

void ScriptingObjects::GraphicsObject::drawRoundedRectangle(var area, float cornerSize, float borderSize)
{
    auto cs = SANITIZED(cornerSize);
    auto bs = SANITIZED(borderSize);
	auto ar = getRectangleFromVar(area);
    
	drawActionHandler.addDrawAction(new ScriptedDrawActions::drawRoundedRectangle(ar, bs, cs));
}

void ScriptingObjects::GraphicsObject::drawHorizontalLine(int y, float x1, float x2)
{
	drawActionHandler.addDrawAction(new ScriptedDrawActions::drawHorizontalLine(y, SANITIZED(x1), SANITIZED(x2)));
}

void ScriptingObjects::GraphicsObject::setOpacity(float alphaValue)
{
	drawActionHandler.addDrawAction(new ScriptedDrawActions::setOpacity(alphaValue));
}

void ScriptingObjects::GraphicsObject::drawLine(float x1, float x2, float y1, float y2, float lineThickness)
{
	drawActionHandler.addDrawAction(new ScriptedDrawActions::drawLine(
		SANITIZED(x1), SANITIZED(y1), SANITIZED(x2), SANITIZED(y2), SANITIZED(lineThickness)));
}

void ScriptingObjects::GraphicsObject::setColour(var colour)
{
	auto c = ScriptingApi::Content::Helpers::getCleanedObjectColour(colour);
	drawActionHandler.addDrawAction(new ScriptedDrawActions::setColour(c));
}

void ScriptingObjects::GraphicsObject::setFont(String fontName, float fontSize)
{
	MainController *mc = getScriptProcessor()->getMainController_();
	auto f = mc->getFontFromString(fontName, SANITIZED(fontSize));
	drawActionHandler.addDrawAction(new ScriptedDrawActions::setFont(f));
}

void ScriptingObjects::GraphicsObject::drawText(String text, var area)
{
	Rectangle<float> r = getRectangleFromVar(area);
	drawActionHandler.addDrawAction(new ScriptedDrawActions::drawText(text, r));
}

void ScriptingObjects::GraphicsObject::drawAlignedText(String text, var area, String alignment)
{
	Rectangle<float> r = getRectangleFromVar(area);

	Result re = Result::ok();
	auto just= ApiHelpers::getJustification(alignment, &re);

	if (re.failed())
		reportScriptError(re.getErrorMessage());

	drawActionHandler.addDrawAction(new ScriptedDrawActions::drawText(text, r, just));
}

void ScriptingObjects::GraphicsObject::setGradientFill(var gradientData)
{
	if (gradientData.isArray())
	{
		Array<var>* data = gradientData.getArray();

		if (gradientData.getArray()->size() == 6)
		{
			auto c1 = ScriptingApi::Content::Helpers::getCleanedObjectColour(data->getUnchecked(0));
			auto c2 = ScriptingApi::Content::Helpers::getCleanedObjectColour(data->getUnchecked(3));

			auto grad = ColourGradient(c1, (float)data->getUnchecked(1), (float)data->getUnchecked(2),
					 					     c2, (float)data->getUnchecked(4), (float)data->getUnchecked(5), false);


			drawActionHandler.addDrawAction(new ScriptedDrawActions::setGradientFill(grad));
		}
		else
			reportScriptError("Gradient Data must have six elements");
	}
	else
		reportScriptError("Gradient Data is not sufficient");
}



void ScriptingObjects::GraphicsObject::drawEllipse(var area, float lineThickness)
{
	drawActionHandler.addDrawAction(new ScriptedDrawActions::drawEllipse(getRectangleFromVar(area), lineThickness));
}

void ScriptingObjects::GraphicsObject::fillEllipse(var area)
{
	drawActionHandler.addDrawAction(new ScriptedDrawActions::fillEllipse(getRectangleFromVar(area)));
}

void ScriptingObjects::GraphicsObject::drawImage(String imageName, var area, int /*xOffset*/, int yOffset)
{
	auto sc = dynamic_cast<ScriptingApi::Content::ScriptPanel*>(parent);
	const Image img = sc->getLoadedImage(imageName);

	if (img.isValid())
	{
		Rectangle<float> r = getRectangleFromVar(area);

        if(r.getWidth() != 0)
        {
            const double scaleFactor = (double)img.getWidth() / (double)r.getWidth();
            
			drawActionHandler.addDrawAction(new ScriptedDrawActions::drawImage(img, r, (float)scaleFactor, yOffset));
        }        
	}
	else
		reportScriptError("Image not found");
}

void ScriptingObjects::GraphicsObject::drawDropShadow(var area, var colour, int radius)
{
	auto r = getIntRectangleFromVar(area);
	DropShadow shadow;

	shadow.colour = ScriptingApi::Content::Helpers::getCleanedObjectColour(colour);
	shadow.radius = radius;

	drawActionHandler.addDrawAction(new ScriptedDrawActions::drawDropShadow(r, shadow));
}

void ScriptingObjects::GraphicsObject::drawTriangle(var area, float angle, float lineThickness)
{
	Path p;
	p.startNewSubPath(0.5f, 0.0f);
	p.lineTo(1.0f, 1.0f);
	p.lineTo(0.0f, 1.0f);
	p.closeSubPath();
	p.applyTransform(AffineTransform::rotation(angle));
	auto r = getRectangleFromVar(area);
	p.scaleToFit(r.getX(), r.getY(), r.getWidth(), r.getHeight(), false);
	
	drawActionHandler.addDrawAction(new ScriptedDrawActions::drawPath(p, lineThickness));
}

void ScriptingObjects::GraphicsObject::fillTriangle(var area, float angle)
{
	Path p;
	p.startNewSubPath(0.5f, 0.0f);
	p.lineTo(1.0f, 1.0f);
	p.lineTo(0.0f, 1.0f);
	p.closeSubPath();
	p.applyTransform(AffineTransform::rotation(angle));
	auto r = getRectangleFromVar(area);
	p.scaleToFit(r.getX(), r.getY(), r.getWidth(), r.getHeight(), false);

	drawActionHandler.addDrawAction(new ScriptedDrawActions::fillPath(p));
}

void ScriptingObjects::GraphicsObject::addDropShadowFromAlpha(var colour, int radius)
{
	DropShadow shadow;

	shadow.colour = ScriptingApi::Content::Helpers::getCleanedObjectColour(colour);
	shadow.radius = radius;

	drawActionHandler.addDrawAction(new ScriptedDrawActions::addDropShadowFromAlpha(shadow));
}

void ScriptingObjects::GraphicsObject::fillPath(var path, var area)
{
	if (PathObject* pathObject = dynamic_cast<PathObject*>(path.getObject()))
	{
		Path p = pathObject->getPath();

		if (area.isArray())
		{
			Rectangle<float> r = getRectangleFromVar(area);
			p.scaleToFit(r.getX(), r.getY(), r.getWidth(), r.getHeight(), false);
		}

		drawActionHandler.addDrawAction(new ScriptedDrawActions::fillPath(p));
	}
}

void ScriptingObjects::GraphicsObject::drawPath(var path, var area, var thickness)
{
	if (PathObject* pathObject = dynamic_cast<PathObject*>(path.getObject()))
	{
		Path p = pathObject->getPath();
		
		if (area.isArray())
		{
			Rectangle<float> r = getRectangleFromVar(area);
			p.scaleToFit(r.getX(), r.getY(), r.getWidth(), r.getHeight(), false);
		}

        auto t = (float)thickness;
		drawActionHandler.addDrawAction(new ScriptedDrawActions::drawPath(p, SANITIZED(t)));
	}
}

void ScriptingObjects::GraphicsObject::rotate(var angleInRadian, var center)
{
	Point<float> c = getPointFromVar(center);
    auto air = (float)angleInRadian;
	auto a = AffineTransform::rotation(SANITIZED(air), c.getX(), c.getY());

	drawActionHandler.addDrawAction(new ScriptedDrawActions::addTransform(a));
}

Point<float> ScriptingObjects::GraphicsObject::getPointFromVar(const var& data)
{
	Point<float>&& f = ApiHelpers::getPointFromVar(data, &rectangleResult);

	if (rectangleResult.failed()) reportScriptError(rectangleResult.getErrorMessage());

	return f;
}

Rectangle<float> ScriptingObjects::GraphicsObject::getRectangleFromVar(const var &data)
{
	Rectangle<float>&& f = ApiHelpers::getRectangleFromVar(data, &rectangleResult);

	if (rectangleResult.failed()) reportScriptError(rectangleResult.getErrorMessage());

	return f;
}

Rectangle<int> ScriptingObjects::GraphicsObject::getIntRectangleFromVar(const var &data)
{
	Rectangle<int>&& f = ApiHelpers::getIntRectangleFromVar(data, &rectangleResult);

	if (rectangleResult.failed()) reportScriptError(rectangleResult.getErrorMessage());

	return f;
}

struct ScriptingObjects::ScriptingMessageHolder::Wrapper
{
	API_VOID_METHOD_WRAPPER_1(ScriptingMessageHolder, setNoteNumber);
	API_VOID_METHOD_WRAPPER_1(ScriptingMessageHolder, setVelocity);
	API_VOID_METHOD_WRAPPER_1(ScriptingMessageHolder, setControllerNumber);
	API_VOID_METHOD_WRAPPER_1(ScriptingMessageHolder, setControllerValue);
	API_METHOD_WRAPPER_0(ScriptingMessageHolder, getNoteNumber);
	API_METHOD_WRAPPER_0(ScriptingMessageHolder, getVelocity);
	API_METHOD_WRAPPER_0(ScriptingMessageHolder, getControllerNumber);
	API_METHOD_WRAPPER_0(ScriptingMessageHolder, getControllerValue);
	API_VOID_METHOD_WRAPPER_1(ScriptingMessageHolder, ignoreEvent);
	API_METHOD_WRAPPER_0(ScriptingMessageHolder, getEventId);
	API_METHOD_WRAPPER_0(ScriptingMessageHolder, getChannel);
	API_VOID_METHOD_WRAPPER_1(ScriptingMessageHolder, setChannel);
	API_VOID_METHOD_WRAPPER_1(ScriptingMessageHolder, setTransposeAmount);
	API_METHOD_WRAPPER_0(ScriptingMessageHolder, getTransposeAmount);
	API_VOID_METHOD_WRAPPER_1(ScriptingMessageHolder, setCoarseDetune);
	API_METHOD_WRAPPER_0(ScriptingMessageHolder, getCoarseDetune);
	API_VOID_METHOD_WRAPPER_1(ScriptingMessageHolder, setFineDetune);
	API_METHOD_WRAPPER_0(ScriptingMessageHolder, getFineDetune);
	API_VOID_METHOD_WRAPPER_1(ScriptingMessageHolder, setGain);
	API_METHOD_WRAPPER_0(ScriptingMessageHolder, getGain);
	API_METHOD_WRAPPER_0(ScriptingMessageHolder, getTimestamp);
	API_METHOD_WRAPPER_0(ScriptingMessageHolder, dump);
};

ScriptingObjects::ScriptingMessageHolder::ScriptingMessageHolder(ProcessorWithScriptingContent* pwsc) :
	ConstScriptingObject(pwsc, (int)HiseEvent::Type::numTypes)
{
	ADD_API_METHOD_1(setNoteNumber);
	ADD_API_METHOD_1(setVelocity);
	ADD_API_METHOD_1(setControllerNumber);
	ADD_API_METHOD_1(setControllerValue);
	ADD_API_METHOD_0(getControllerNumber);
	ADD_API_METHOD_0(getControllerValue);
	ADD_API_METHOD_0(getNoteNumber);
	ADD_API_METHOD_0(getVelocity);
	ADD_API_METHOD_1(ignoreEvent);
	ADD_API_METHOD_0(getEventId);
	ADD_API_METHOD_0(getChannel);
	ADD_API_METHOD_1(setChannel);
	ADD_API_METHOD_0(getGain);
	ADD_API_METHOD_1(setGain);
	ADD_API_METHOD_1(setTransposeAmount);
	ADD_API_METHOD_0(getTransposeAmount);
	ADD_API_METHOD_1(setCoarseDetune);
	ADD_API_METHOD_0(getCoarseDetune);
	ADD_API_METHOD_1(setFineDetune);
	ADD_API_METHOD_0(getFineDetune);
	ADD_API_METHOD_0(getTimestamp);
	ADD_API_METHOD_0(dump);

	addConstant("Empty", 0);
	addConstant("NoteOn", 1);
	addConstant("NoteOff", 2);
	addConstant("Controller", 3);
	addConstant("PitchBend", 4);
	addConstant("Aftertouch", 5);
	addConstant("AllNotesOff", 6);
	addConstant("SongPosition", 7);
	addConstant("MidiStart", 8);
	addConstant("MidiStop", 9);
	addConstant("VolumeFade", 10);
	addConstant("PitchFade", 11);
	addConstant("TimerEvent", 12);
}

int ScriptingObjects::ScriptingMessageHolder::getNoteNumber() const { return (int)e.getNoteNumber(); }
var ScriptingObjects::ScriptingMessageHolder::getControllerNumber() const { return (int)e.getControllerNumber(); }
var ScriptingObjects::ScriptingMessageHolder::getControllerValue() const { return (int)e.getControllerNumber(); }
int ScriptingObjects::ScriptingMessageHolder::getChannel() const { return (int)e.getChannel(); }
void ScriptingObjects::ScriptingMessageHolder::setChannel(int newChannel) { e.setChannel(newChannel); }
void ScriptingObjects::ScriptingMessageHolder::setNoteNumber(int newNoteNumber) { e.setNoteNumber(newNoteNumber); }
void ScriptingObjects::ScriptingMessageHolder::setVelocity(int newVelocity) { e.setVelocity((uint8)newVelocity); }
void ScriptingObjects::ScriptingMessageHolder::setControllerNumber(int newControllerNumber) { e.setControllerNumber(newControllerNumber);}
void ScriptingObjects::ScriptingMessageHolder::setControllerValue(int newControllerValue) { e.setControllerValue(newControllerValue); }
int ScriptingObjects::ScriptingMessageHolder::getVelocity() const { return e.getVelocity(); }
void ScriptingObjects::ScriptingMessageHolder::ignoreEvent(bool shouldBeIgnored /*= true*/) { e.ignoreEvent(shouldBeIgnored); }
int ScriptingObjects::ScriptingMessageHolder::getEventId() const { return (int)e.getEventId(); }
void ScriptingObjects::ScriptingMessageHolder::setTransposeAmount(int tranposeValue) { e.setTransposeAmount(tranposeValue); }
int ScriptingObjects::ScriptingMessageHolder::getTransposeAmount() const { return (int)e.getTransposeAmount(); }
void ScriptingObjects::ScriptingMessageHolder::setCoarseDetune(int semiToneDetune) { e.setCoarseDetune(semiToneDetune); }
int ScriptingObjects::ScriptingMessageHolder::getCoarseDetune() const { return (int)e.getCoarseDetune(); }
void ScriptingObjects::ScriptingMessageHolder::setFineDetune(int cents) { e.setFineDetune(cents); }
int ScriptingObjects::ScriptingMessageHolder::getFineDetune() const { return (int)e.getFineDetune(); }
void ScriptingObjects::ScriptingMessageHolder::setGain(int gainInDecibels) { e.setGain(gainInDecibels); }
int ScriptingObjects::ScriptingMessageHolder::getGain() const { return (int)e.getGain(); }
int ScriptingObjects::ScriptingMessageHolder::getTimestamp() const { return (int)e.getTimeStamp(); }
void ScriptingObjects::ScriptingMessageHolder::setTimestamp(int timestampSamples) { e.setTimeStamp(timestampSamples);}
void ScriptingObjects::ScriptingMessageHolder::addToTimestamp(int deltaSamples) { e.addToTimeStamp((int16)deltaSamples); }

String ScriptingObjects::ScriptingMessageHolder::dump() const
{
	String x;
	x << "Type: " << e.getTypeAsString() << ", ";
	x << "Number: " << String(e.getNoteNumber()) << ", ";
	x << "Value: " << String(e.getVelocity()) << ", ";
	x << "Channel: " << String(e.getChannel()) << ", ";
	x << "EventId: " << String(e.getEventId()) << ", ";
	x << "Timestamp: " << String(e.getTimeStamp()) << ", ";

	return x;
}



ApiHelpers::ModuleHandler::ModuleHandler(Processor* parent_, JavascriptProcessor* sp) :
	parent(parent_),
	scriptProcessor(sp)
{
#if USE_BACKEND

	auto console = parent != nullptr ? parent->getMainController()->getConsoleHandler().getMainConsole() : nullptr;

	if (console)
		mainEditor = GET_BACKEND_ROOT_WINDOW(console);

#else
	mainEditor = nullptr;
#endif
}

ApiHelpers::ModuleHandler::~ModuleHandler()
{
	
}




bool ApiHelpers::ModuleHandler::removeModule(Processor* p)
{
	if (p == nullptr)
		return true;

	if (p->getMainController()->getKillStateHandler().getCurrentThread() == MainController::KillStateHandler::AudioThread)
	{
		throw String("Effects can't be removed from the audio thread!");
	}

	if (p != nullptr)
	{
		auto removeFunction = [](Processor* p)
		{
			auto c = dynamic_cast<Chain*>(p->getParentProcessor(false));

			jassert(c != nullptr);

			if (c == nullptr)
				return SafeFunctionCall::cancelled;

			// Remove it but don't delete it
			c->getHandler()->remove(p, false);

			return SafeFunctionCall::OK;
		};

		parent->getMainController()->getGlobalAsyncModuleHandler().removeAsync(p, removeFunction);
		return true;
	}
	else
		return false;
}

Processor* ApiHelpers::ModuleHandler::addModule(Chain* c, const String& type, const String& id, int index /*= -1*/)
{
	WARN_IF_AUDIO_THREAD(true, IllegalAudioThreadOps::HeapBlockAllocation);

	for (int i = 0; i < c->getHandler()->getNumProcessors(); i++)
	{
		if (c->getHandler()->getProcessor(i)->getId() == id)
		{
			return c->getHandler()->getProcessor(i);
		}
	}

	SuspendHelpers::ScopedTicket ticket(parent->getMainController());

	parent->getMainController()->getJavascriptThreadPool().killVoicesAndExtendTimeOut(getScriptProcessor());

	LockHelpers::freeToGo(parent->getMainController());

	ScopedPointer<Processor> newProcessor = parent->getMainController()->createProcessor(c->getFactoryType(), type, id);

	if (newProcessor == nullptr)
		throw String("Module with type " + type + " could not be generated.");

	// Now we're safe...
	Processor* pFree = newProcessor.release();

	auto addFunction = [c, index](Processor* p)
	{
		if (c == nullptr)
		{
			delete p; // Rather bad...
			jassertfalse;
			return SafeFunctionCall::OK;
		}

		if (index >= 0 && index < c->getHandler()->getNumProcessors())
		{
			Processor* sibling = c->getHandler()->getProcessor(index);
			c->getHandler()->add(p, sibling);
		}
		else
			c->getHandler()->add(p, nullptr);

		return SafeFunctionCall::OK;
	};
	




	parent->getMainController()->getGlobalAsyncModuleHandler().addAsync(pFree, addFunction);

	// will be owned by the job, then by the handler...
	return pFree;
}

hise::Modulator* ApiHelpers::ModuleHandler::addAndConnectToGlobalModulator(Chain* c, Modulator* globalModulator, const String& modName, bool connectAsStaticMod/*=false*/)
{
	if (globalModulator == nullptr)
		throw String("Global Modulator does not exist");

	if (auto container = dynamic_cast<GlobalModulatorContainer*>(ProcessorHelpers::findParentProcessor(globalModulator, true)))
	{
		GlobalModulator* m = nullptr;

		if (dynamic_cast<VoiceStartModulator*>(globalModulator) != nullptr)
		{
			auto vMod = addModule(c, GlobalVoiceStartModulator::getClassType().toString(), modName);
			m = dynamic_cast<GlobalModulator*>(vMod);
		}
		else if (dynamic_cast<TimeVariantModulator*>(globalModulator) != nullptr)
		{
			if (connectAsStaticMod)
			{
				auto tMod = addModule(c, GlobalStaticTimeVariantModulator::getClassType().toString(), modName);
				m = dynamic_cast<GlobalModulator*>(tMod);
			}
			else
			{
				auto tMod = addModule(c, GlobalTimeVariantModulator::getClassType().toString(), modName);
				m = dynamic_cast<GlobalModulator*>(tMod);
			}
		}
		else
			throw String("Not a global modulator");

		if (m == nullptr)
			throw String("Global modulator can't be created");

		auto entry = container->getId() + ":" + globalModulator->getId();

		m->connectToGlobalModulator(entry);

		if (!m->isConnected())
		{
			throw String("Can't connect to global modulator");
		}

		auto returnMod = dynamic_cast<Modulator*>(m);

#if USE_BACKEND
		returnMod->sendChangeMessage();
#endif

		return returnMod;
	}
	else
		throw String("The modulator you passed in is not a global modulator. You must specify a modulator in a Global Modulator Container");
}

struct ScriptingObjects::ExpansionObject::Wrapper
{
	API_METHOD_WRAPPER_0(ExpansionObject, getSampleMapList);
	API_METHOD_WRAPPER_0(ExpansionObject, getAudioFileList);
	API_METHOD_WRAPPER_0(ExpansionObject, getImageFilelist);
	API_METHOD_WRAPPER_1(ExpansionObject, getReferenceString);
};

ScriptingObjects::ExpansionObject::ExpansionObject(ProcessorWithScriptingContent* p, Expansion* expansion) :
	ConstScriptingObject(p, 5),
	data(expansion)
{
	addConstant("Name", expansion->name.get());
	addConstant("Version", expansion->version.get());
	addConstant("ProjectName", expansion->projectName.get());
	addConstant("ProjectVersion", expansion->projectVersion.get());
	addConstant("Encrypted", expansion->isEncrypted);

	ADD_API_METHOD_0(getSampleMapList);
	ADD_API_METHOD_0(getAudioFileList);
	ADD_API_METHOD_0(getImageFilelist);
	ADD_API_METHOD_1(getReferenceString);
}

var ScriptingObjects::ExpansionObject::getSampleMapList()
{
	if (objectExists())
	{
		return data->getSampleMapList();
	}

	return var();
}


var ScriptingObjects::ExpansionObject::getAudioFileList()
{
	if (objectExists())
	{
		return data->getAudioFileList();
	}

	return var();
}

var ScriptingObjects::ExpansionObject::getImageFilelist()
{
	if (objectExists())
	{
		return data->getImageList();
	}

	return var();
}

var ScriptingObjects::ExpansionObject::getReferenceString(var relativeFilePath)
{
	if (objectExists())
	{
		String s;

		s << "{EXP::" << data->name << "}" << relativeFilePath.toString();

		return var(s);
	}

	return var();
}

struct ScriptingObjects::ExpansionHandlerObject::Wrapper
{
	API_METHOD_WRAPPER_0(ExpansionHandlerObject, getExpansionList);
	API_METHOD_WRAPPER_0(ExpansionHandlerObject, getCurrentExpansion);
	API_VOID_METHOD_WRAPPER_1(ExpansionHandlerObject, setLoadingCallback);
	API_METHOD_WRAPPER_1(ExpansionHandlerObject, loadExpansion);
};

ScriptingObjects::ExpansionHandlerObject::ExpansionHandlerObject(ProcessorWithScriptingContent* p) :
	ConstScriptingObject(p, 0),
	handler(p->getMainController_()->getExpansionHandler())
{
	handler.addListener(this);

	ADD_API_METHOD_0(getExpansionList);
	ADD_API_METHOD_0(getCurrentExpansion);
	ADD_API_METHOD_1(setLoadingCallback);
	ADD_API_METHOD_1(loadExpansion);
}

ScriptingObjects::ExpansionHandlerObject::~ExpansionHandlerObject()
{
	handler.removeListener(this);
}

void ScriptingObjects::ExpansionHandlerObject::expansionPackLoaded(Expansion* currentExpansion)
{
	if (HiseJavascriptEngine::isJavascriptFunction(loadingCallback))
	{
		var expansionArg;

		if (currentExpansion != nullptr)
		{
			auto e = new ExpansionObject(getScriptProcessor(), currentExpansion);
			expansionArg = var(e);
		}
		
		var thisObject(this);
		var::NativeFunctionArgs args(thisObject, &expansionArg, 1);

		Result r = Result::ok();

		auto engine = dynamic_cast<JavascriptMidiProcessor*>(getScriptProcessor())->getScriptEngine();

		if (engine != nullptr)
		{
			engine->maximumExecutionTime = RelativeTime(2.0);
			engine->callExternalFunction(loadingCallback, args, &r);

			if (r.failed())
				debugError(getProcessor(), r.getErrorMessage());
		}
	}
}

var ScriptingObjects::ExpansionHandlerObject::getExpansionList()
{
	Array<var> list;

	for (int i = 0; i < handler.getNumExpansions(); i++)
	{
		auto newObject = new ExpansionObject(getScriptProcessor(), handler.getExpansion(i));

		list.add(var(newObject));
	}

	return var(list);
}

var ScriptingObjects::ExpansionHandlerObject::getCurrentExpansion()
{
	if (auto e = handler.getCurrentExpansion())
	{
		auto newObject = new ExpansionObject(getScriptProcessor(), e);

		return var(newObject);
	}

	return var();
}


void ScriptingObjects::ExpansionHandlerObject::setLoadingCallback(var f)
{
	if (HiseJavascriptEngine::isJavascriptFunction(f))
	{
		loadingCallback = f;
	}
}

bool ScriptingObjects::ExpansionHandlerObject::loadExpansion(const String expansionName)
{
	return handler.setCurrentExpansion(expansionName);
}

} // namespace hise
