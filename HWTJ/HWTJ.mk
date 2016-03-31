##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=HWTJ
ConfigurationName      :=Debug
WorkspacePath          :=/home/tjisana/IEOR4739/HWTJ
ProjectPath            :=/home/tjisana/IEOR4739/HWTJ
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Tjisana Kerr
Date                   :=29/03/16
CodeLitePath           :=/home/tjisana/.codelite
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="HWTJ.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/src_myoalgo.c$(ObjectSuffix) $(IntermediateDirectory)/src_utilities.c$(ObjectSuffix) $(IntermediateDirectory)/src_myopt.c$(ObjectSuffix) $(IntermediateDirectory)/src_myobasic.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/src_myoalgo.c$(ObjectSuffix): ../myoptL6/src/myoalgo.c $(IntermediateDirectory)/src_myoalgo.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/tjisana/IEOR4739/myoptL6/src/myoalgo.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_myoalgo.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_myoalgo.c$(DependSuffix): ../myoptL6/src/myoalgo.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_myoalgo.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_myoalgo.c$(DependSuffix) -MM "../myoptL6/src/myoalgo.c"

$(IntermediateDirectory)/src_myoalgo.c$(PreprocessSuffix): ../myoptL6/src/myoalgo.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_myoalgo.c$(PreprocessSuffix) "../myoptL6/src/myoalgo.c"

$(IntermediateDirectory)/src_utilities.c$(ObjectSuffix): ../myoptL6/src/utilities.c $(IntermediateDirectory)/src_utilities.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/tjisana/IEOR4739/myoptL6/src/utilities.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_utilities.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_utilities.c$(DependSuffix): ../myoptL6/src/utilities.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_utilities.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_utilities.c$(DependSuffix) -MM "../myoptL6/src/utilities.c"

$(IntermediateDirectory)/src_utilities.c$(PreprocessSuffix): ../myoptL6/src/utilities.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_utilities.c$(PreprocessSuffix) "../myoptL6/src/utilities.c"

$(IntermediateDirectory)/src_myopt.c$(ObjectSuffix): ../myoptL6/src/myopt.c $(IntermediateDirectory)/src_myopt.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/tjisana/IEOR4739/myoptL6/src/myopt.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_myopt.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_myopt.c$(DependSuffix): ../myoptL6/src/myopt.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_myopt.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_myopt.c$(DependSuffix) -MM "../myoptL6/src/myopt.c"

$(IntermediateDirectory)/src_myopt.c$(PreprocessSuffix): ../myoptL6/src/myopt.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_myopt.c$(PreprocessSuffix) "../myoptL6/src/myopt.c"

$(IntermediateDirectory)/src_myobasic.c$(ObjectSuffix): ../myoptL6/src/myobasic.c $(IntermediateDirectory)/src_myobasic.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/tjisana/IEOR4739/myoptL6/src/myobasic.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_myobasic.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_myobasic.c$(DependSuffix): ../myoptL6/src/myobasic.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_myobasic.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_myobasic.c$(DependSuffix) -MM "../myoptL6/src/myobasic.c"

$(IntermediateDirectory)/src_myobasic.c$(PreprocessSuffix): ../myoptL6/src/myobasic.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_myobasic.c$(PreprocessSuffix) "../myoptL6/src/myobasic.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


