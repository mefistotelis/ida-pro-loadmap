PROC=loadmap
O1=MAPReader
O2=stdafx

# required for GetAsyncKeyState()
STDLIBS += User32.lib

include ../plugin.mak

# MAKEDEP dependency list ------------------
$(F)loadmap$(O)   : $(I)bitrange.hpp $(I)bytes.hpp $(I)config.hpp $(I)fpro.h  \
	          $(I)funcs.hpp $(I)ida.hpp $(I)idp.hpp $(I)kernwin.hpp     \
	          $(I)lines.hpp $(I)llong.hpp $(I)loader.hpp $(I)nalt.hpp   \
	          $(I)netnode.hpp $(I)pro.h $(I)range.hpp $(I)segment.hpp   \
	          $(I)ua.hpp $(I)xref.hpp \
	          src/loadmap.cpp
$(F)MAPReader$(O)  : src/MAPReader.cpp src/MAPReader.h
$(F)stdafx$(O)  : src/stdafx.cpp src/stdafx.h

$(PROC): NO_OBSOLETE_FUNCS =

$(F)loadmap$(O): NO_OBSOLETE_FUNCS =

# Copy of the standard idasdk rule, but this one allows src directory
$(F)%$(O): src/%.cpp
	$(strip $(QCXX)$(CXX) $(CXXFLAGS) $(NORTTI) $(CONLY) $(OBJSW)$@ $<)
