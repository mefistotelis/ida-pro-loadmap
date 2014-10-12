PROC=LoadMAP
O1=MAPReader
O2=stdafx

IDA ?= ../../
include $(IDA)plugins/plugin.gcc.mak

LDFLAGS += -static-libgcc -static-libstdc++
LIBS += -lshlwapi

# MAKEDEP dependency list ------------------
$(F)LoadMAP$(O): $(I)area.hpp $(I)funcs.hpp $(I)help.h $(I)ida.hpp \
	         $(I)idp.hpp $(I)kernwin.hpp $(I)lines.hpp $(I)llong.hpp \
	         $(I)loader.hpp $(I)nalt.hpp $(I)netnode.hpp $(I)pro.h \
	         $(I)segment.hpp $(I)ua.hpp $(I)xref.hpp \
	         src/loadmap.cpp src/MAPReader.h src/stdafx.h
$(OBJ1):	 src/MAPReader.cpp src/MAPReader.h
$(OBJ2):	 src/stdafx.cpp src/stdafx.h

# The plugin.gcc.mak defines target for cpp files in pwd, here is one for src sub-dir
$(F)%$(O): src/%.cpp
	$(CC) -c $(CFLAGS) $< -o $@
