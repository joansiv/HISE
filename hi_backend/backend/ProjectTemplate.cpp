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
 *   which must be separately licensed for cloused source applications:
 *
 *   http://www.juce.com
 *
 *   ===========================================================================
 */

// This is a autogenerated file that contains the template for the plugin exporter .jucer file

static const unsigned char projectTemplate_jucer_lines[] =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
"\r\n"
"<JUCERPROJECT id=\"Tw64Zd\" name=\"%NAME%\" projectType=\"audioplug\" version=\"%VERSION%\"\r\n"
"              bundleIdentifier=\"%BUNDLE_ID%\" includeBinaryInAppConfig=\"1\" buildVST=\"%BUILD_VST%\"\r\n"
"              buildVST3=\"0\" buildAU=\"%BUILD_AU%\" buildAUv3=\"%BUILD_AUV3%\" buildRTAS=\"0\" buildAAX=\"%BUILD_AAX%\" pluginName=\"%NAME%\"\r\n"
"              pluginDesc=\"%NAME%\" pluginManufacturer=\"%COMPANY%\" pluginManufacturerCode=\"%MC%\"\r\n"
"              pluginCode=\"%PC%\" pluginChannelConfigs=\"%CHANNEL_CONFIG%\" pluginIsSynth=\"%PLUGINISSYNTH%\"\r\n"
"              pluginWantsMidiIn=\"%PLUGINWANTSMIDIIN\" pluginProducesMidiOut=\"0\" pluginIsMidiEffectPlugin=\"0\"\r\n"
"              pluginEditorRequiresKeys=\"1\" pluginAUExportPrefix=\"DemoProjectAU\"\r\n"
"              pluginRTASCategory=\"\" aaxIdentifier=\"%AAX_IDENTIFIER%\"\r\n"
"              pluginAAXCategory=\"%AAX_CATEGORY%\" jucerVersion=\"4.1.0\"\r\n"
"              companyName=\"%COMPANY%\" companyWebsite=\"%COMPANY_WEBSITE%\">\r\n"
"  <MAINGROUP id=\"SLR7uY\" name=\"%NAME%\">\r\n"
"    <GROUP id=\"{122C85F1-8B09-257A-B636-113E3EAC258A}\" name=\"Source\">\r\n"
"      <FILE id=\"eLP6Ii\" name=\"balanceKnob_200.png\" compile=\"0\" resource=\"1\"\r\n"
"            file=\"Source/Images/balanceKnob_200.png\"/>\r\n"
"	   <FILE id=\"O9wjCd\" name=\"FrontendKnob_Bipolar.png\" compile=\"0\" resource=\"1\""
"            file=\"Source/Images/FrontendKnob_Bipolar.png\"/>"
"	   <FILE id=\"UAxUWd\" name=\"FrontendKnob_Unipolar.png\" compile=\"0\" resource=\"1\""
"            file=\"Source/Images/FrontendKnob_Unipolar.png\"/>"
"	   <FILE id=\"KmyOlv\" name=\"infoError.png\" compile=\"0\" resource=\"1\" file=\"Source/Images/infoError.png\"/>\r\n"
"      <FILE id=\"CNpA5r\" name=\"infoInfo.png\" compile=\"0\" resource=\"1\" file=\"Source/Images/infoInfo.png\"/>\r\n"
"      <FILE id=\"MjbOy3\" name=\"infoQuestion.png\" compile=\"0\" resource=\"1\"\r\n"
"            file=\"Source/Images/infoQuestion.png\"/>\r\n"
"      <FILE id=\"zsTpOz\" name=\"infoWarning.png\" compile=\"0\" resource=\"1\" file=\"Source/Images/infoWarning.png\"/>\r\n"
"      <FILE id=\"d1rhrw\" name=\"knobEmpty_200.png\" compile=\"0\" resource=\"1\"\r\n"
"            file=\"Source/Images/knobEmpty_200.png\"/>\r\n"
"      <FILE id=\"u2SLs3\" name=\"knobModulated_200.png\" compile=\"0\" resource=\"1\"\r\n"
"            file=\"Source/Images/knobModulated_200.png\"/>\r\n"
"      <FILE id=\"rTpn8D\" name=\"knobUnmodulated_200.png\" compile=\"0\" resource=\"1\"\r\n"
"            file=\"Source/Images/knobUnmodulated_200.png\"/>\r\n"
"      <FILE id=\"jJzDA2\" name=\"CopyProtection.cpp\" compile=\"1\" resource=\"0\" file=\"Source/CopyProtection.cpp\"/>\r\n"
"      <FILE id=\"AOT2K8\" name=\"Plugin.cpp\" compile=\"1\" resource=\"0\" file=\"Source/Plugin.cpp\"/>\r\n"
"      <FILE id=\"esGQuC\" name=\"PresetData.cpp\" compile=\"1\" resource=\"0\" file=\"Source/PresetData.cpp\"/>\r\n"
"      <FILE id=\"q8WZ82\" name=\"PresetData.h\" compile=\"0\" resource=\"0\" file=\"Source/PresetData.h\"/>\r\n"
"      <FILE id=\"jjzOA2\" name=\"toggle_200.png\" compile=\"0\" resource=\"1\" file=\"Source/Images/toggle_200.png\"/>\r\n"
"%ADDITIONAL_FILES%"	
"    </GROUP>\r\n"
"  </MAINGROUP>\r\n"
"  <EXPORTFORMATS>\r\n"
"    <%VS_VERSION% targetFolder=\"Builds/%TARGET_FOLDER%\" vstFolder=\"%VSTSDK_FOLDER%\" aaxFolder=\"%AAX_PATH%\" \r\n"
"            IPPLibrary=\"%IPP_WIN_SETTING%\" extraDefs=\"%EXTRA_DEFINES_WIN%\" >\r\n"
"      <CONFIGURATIONS>\r\n"
"        <CONFIGURATION name=\"Debug\" winWarningLevel=\"4\" generateManifest=\"1\" winArchitecture=\"32-bit\"\r\n"
"                       libraryPath=\"%AAX_DEBUG_LIB%;%WIN_STATIC_LIB_FOLDER_D32%/\" isDebug=\"1\" optimisation=\"1\" targetName=\"%NAME%_Debug x86.dll\" binaryPath=\"Compiled/\" useRuntimeLibDLL=\"0\" prebuildCommand=\"%PREBUILD_COMMAND%\"/>\r\n"
"        <CONFIGURATION name=\"Debug\" winWarningLevel=\"4\" generateManifest=\"1\" winArchitecture=\"x64\"\r\n"
"                       isDebug=\"1\" optimisation=\"1\" targetName=\"%NAME%_Debug_x64.dll\"\r\n"
"                       libraryPath=\"%AAX_DEBUG_LIB%;%WIN_STATIC_LIB_FOLDER_D64%\" binaryPath=\"Compiled/\" useRuntimeLibDLL=\"0\" prebuildCommand=\"%PREBUILD_COMMAND%\"/>\r\n"
"        <CONFIGURATION name=\"Release\" winWarningLevel=\"4\" generateManifest=\"1\" winArchitecture=\"32-bit\"\r\n"
"                       libraryPath=\"%AAX_RELEASE_LIB%;%WIN_STATIC_LIB_FOLDER_R32%\" isDebug=\"0\" optimisation=\"3\" targetName=\"%NAME% x86.dll\" binaryPath=\"Compiled/\" useRuntimeLibDLL=\"0\" prebuildCommand=\"%PREBUILD_COMMAND%\"/>\r\n"
"        <CONFIGURATION name=\"Release\" winWarningLevel=\"4\" generateManifest=\"1\"\r\n"
"                       winArchitecture=\"x64\" isDebug=\"0\" optimisation=\"3\" targetName=\"%NAME% x64.dll\"\r\n"
"                       libraryPath=\"%AAX_RELEASE_LIB%;%WIN_STATIC_LIB_FOLDER_R64%\" binaryPath=\"Compiled/\" useRuntimeLibDLL=\"0\" prebuildCommand=\"%PREBUILD_COMMAND%\"/>\r\n"
"      </CONFIGURATIONS>\r\n"
"      <MODULEPATHS>\r\n"
"        <MODULEPATH id=\"juce_core\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_events\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_graphics\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_data_structures\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_gui_basics\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_gui_extra\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_cryptography\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_opengl\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_audio_basics\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_audio_devices\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_audio_formats\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_audio_processors\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_audio_plugin_client\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_tracktion_marketplace\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_audio_utils\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_dsp_library\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_frontend\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_modules\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_scripting\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_core\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_lac\" path=\"%HISE_PATH%\"/>\r\n"
"		 <MODULEPATH id=\"hi_sampler\" path=\"%HISE_PATH%\"/>\r\n"
"	     <MODULEPATH id=\"hi_components\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_dsp\" path=\"%HISE_PATH%\"/>\r\n"
"      </MODULEPATHS>\r\n"
"    </%VS_VERSION%>\r\n"
"    <XCODE_MAC targetFolder=\"Builds/MacOSX\"  vstFolder=\"%VSTSDK_FOLDER%\"  aaxFolder=\"%AAX_PATH%\" extraCompilerFlags=\"-Wno-reorder -Wno-inconsistent-missing-override\"\r\n"
"               customPList=\"&lt;?xml version=&quot;1.0&quot; encoding=&quot;UTF-8&quot;?&gt;&#10;&lt;!DOCTYPE plist PUBLIC &quot;-//Apple//DTD PLIST 1.0//EN&quot; &quot;http://www.apple.com/DTDs/PropertyList-1.0.dtd&quot;&gt;&#10;&lt;plist version=&"
"quot;1.0&quot;&gt;&#10;&lt;dict&gt;&#10;&lt;key&gt;NSAppTransportSecurity&lt;/key&gt; &#10;&lt;dict&gt; &#10;&lt;key&gt;NSAllowsArbitraryLoads&lt;/key&gt;&lt;true/&gt;&#10;&lt;NSHumanReadableCopyright&lt;/key&gt;&#9;&lt;string&gt;%COPY_RIGHT_NOTICE%&lt;/string&gt;&#10;&lt;/dict&gt;&#10;&lt;/dict&gt;&#10;&lt;/plist&gt;\"\r\n"
"               extraLinkerFlags=\"%IPP_COMPILER_FLAGS% %OSX_STATIC_LIBS%\" extraDefs=\"%EXTRA_DEFINES_OSX%\">\r\n"
"                \r\n"
"      <CONFIGURATIONS>\r\n"
"        <CONFIGURATION name=\"Debug\" osxSDK=\"default\" osxCompatibility=\"10.7 SDK\" osxArchitecture=\"64BitUniversal\"\r\n"
"                       isDebug=\"1\" optimisation=\"1\" targetName=\"%NAME% Debug\" headerPath=\"%IPP_HEADER%\"\r\n"
"                       libraryPath=\"%IPP_LIBRARY%%AAX_DEBUG_LIB%\" cppLibType=\"libc++\"/>\r\n"
"        <CONFIGURATION name=\"Release\" osxSDK=\"default\" osxCompatibility=\"10.7 SDK\" osxArchitecture=\"64BitUniversal\"\r\n"
"                       isDebug=\"0\" optimisation=\"3\" targetName=\"%NAME%\" headerPath=\"%IPP_HEADER%\"\r\n"
"                       libraryPath=\"%IPP_LIBRARY%%AAX_RELEASE_LIB%\" cppLibType=\"libc++\" linkTimeOptimisation=\"1\"\r\n"
"                       cppLanguageStandard=\"c++11\" stripLocalSymbols=\"1\"/>\r\n"
"      </CONFIGURATIONS>\r\n"
"      <MODULEPATHS>\r\n"
"        <MODULEPATH id=\"juce_tracktion_marketplace\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_opengl\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_gui_extra\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_gui_basics\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_graphics\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_events\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_data_structures\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_cryptography\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_core\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_audio_utils\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_audio_processors\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_audio_plugin_client\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_audio_formats\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_audio_devices\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_audio_basics\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_modules\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_dsp_library\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_frontend\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_scripting\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_core\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_lac\" path=\"%HISE_PATH%\"/>\r\n"
"		 <MODULEPATH id=\"hi_sampler\" path=\"%HISE_PATH%\"/>\r\n"
"	     <MODULEPATH id=\"hi_components\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_dsp\" path=\"%HISE_PATH%\"/>\r\n"
"      </MODULEPATHS>\r\n"
"    </XCODE_MAC>\r\n"
"    <XCODE_IPHONE targetFolder=\"Builds/iOS\" iosScreenOrientation=\"landscape\" extraCompilerFlags=\"-Wno-reorder -Wno-inconsistent-missing-override\" extraLinkerFlags=\"%OSX_STATIC_LIBS%\" extraDefs=\"%EXTRA_DEFINES_IOS%&#10;USE_IPP=0&#10;HISE_IOS=1&#10;JUCE_USE_CUSTOM_AU3_STANDALONE_APP=1&#10;USE_VDSP_FFT=1\" %ICON_FILE% \r\n"
"                  UIStatusBarHidden=\"1\" iosBackgroundAudio=\"1\" iosBackgroundBle=\"0\""
"   customXcodeResourceFolders=\"../Samples\n../Images\n../AudioFiles\n../SampleMaps\n../UserPresets\""
"   duplicateResourceFolderForAUv3AppEx=\"%USE_APP_GROUPS%\" appGroupId=\"%APP_GROUP_ID%\""
"   iosDevelopmentTeamID=\"%DEVELOPMENT_TEAM_ID%\">\r\n"
"      <CONFIGURATIONS>\r\n"
"        <CONFIGURATION name=\"Debug\" iosCompatibility=\"9.0\" isDebug=\"1\" optimisation=\"1\"\r\n"
"                       targetName=\"%NAME%\" iosTargetFamily=\"%TARGET_FAMILY%\" cppLanguageStandard=\"c++11\" cppLibType=\"libc++\"/>\r\n"
"        <CONFIGURATION name=\"Release\" iosCompatibility=\"9.0\" isDebug=\"0\" optimisation=\"3\"\r\n"
"                       targetName=\"%NAME%\" iosTargetFamily=\"%TARGET_FAMILY%\" cppLanguageStandard=\"c++11\" cppLibType=\"libc++\"\r\n"
"                       linkTimeOptimisation=\"1\" />\r\n"
"      </CONFIGURATIONS>\r\n"
"      <MODULEPATHS>\r\n"
"        <MODULEPATH id=\"juce_tracktion_marketplace\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_opengl\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_gui_extra\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_gui_basics\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_graphics\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_events\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_data_structures\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_cryptography\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_core\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_audio_utils\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_audio_processors\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_audio_formats\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_audio_devices\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"juce_audio_basics\" path=\"%JUCE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_scripting\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_modules\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_frontend\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_dsp_library\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_core\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_lac\" path=\"%HISE_PATH%\"/>\r\n"
"		 <MODULEPATH id=\"hi_sampler\" path=\"%HISE_PATH%\"/>\r\n"
"	     <MODULEPATH id=\"hi_components\" path=\"%HISE_PATH%\"/>\r\n"
"        <MODULEPATH id=\"hi_dsp\" path=\"%HISE_PATH%\"/>\r\n"
"      </MODULEPATHS>\r\n"
"    </XCODE_IPHONE>\r\n"
"  </EXPORTFORMATS>\r\n"
"  <MODULES>\r\n"
"    <MODULE id=\"hi_core\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"hi_frontend\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"hi_scripting\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"hi_modules\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"hi_dsp_library\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"hi_lac\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"hi_sampler\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"hi_components\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"hi_dsp\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"juce_audio_basics\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"juce_audio_devices\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"juce_audio_formats\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"juce_audio_plugin_client\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"juce_audio_processors\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"juce_audio_utils\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"juce_core\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"juce_cryptography\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"juce_data_structures\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"juce_events\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"juce_graphics\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"juce_gui_basics\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"juce_gui_extra\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"juce_opengl\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"    <MODULE id=\"juce_tracktion_marketplace\" showAllCode=\"1\" useLocalCopy=\"0\"/>\r\n"
"  </MODULES>\r\n"
"  <JUCEOPTIONS JUCE_QUICKTIME=\"disabled\" USE_BACKEND=\"disabled\" USE_FRONTEND=\"enabled\" HI_EXPORT_DSP_LIBRARY=\"disabled\" USE_COPY_PROTECTION=\"%USE_COPY_PROTECTION%\" IS_STANDALONE_APP=\"disabled\"\r\n"
"               USE_IPP=\"%USE_IPP%\" FRONTEND_IS_PLUGIN=\"%FRONTEND_IS_PLUGIN%\" USE_CUSTOM_FRONTEND_TOOLBAR=\"%USE_CUSTOM_FRONTEND_TOOLBAR%\" IS_STANDALONE_FRONTEND=\"%IS_STANDALONE_FRONTEND%\" USE_GLITCH_DETECTION = \"enabled\" ENABLE_PLOTTER=\"disabled\" ENABLE_SCRIPTING_SAFE_CHECKS=\"disabled\""
"               ENABLE_ALL_PEAK_METERS=\"disabled\" USE_SPLASH_SCREEN=\"%USE_SPLASH_SCREEN%\" ENABLE_CONSOLE_OUTPUT=\"disabled\" HLAC_MEASURE_DECODING_PERFORMANCE=\"disabled\" HLAC_DEBUG_LOG=\"disabled\" HLAC_INCLUDE_TEST_SUITE=\"disabled\"/>/>\r\n"
"</JUCERPROJECT>\r\n";

const char* projectTemplate_jucer = (const char*)projectTemplate_jucer_lines;


