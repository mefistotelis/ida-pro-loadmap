PROC=loadmap

# we explicitly added our module targets
NO_DEFAULT_TARGETS = 1

include ../plugin.mak

# MAKEDEP dependency list ------------------
$(F)loadmap$(O)   : $(I)bitrange.hpp $(I)bytes.hpp $(I)config.hpp $(I)fpro.h  \
	          $(I)funcs.hpp $(I)ida.hpp $(I)idp.hpp $(I)kernwin.hpp     \
	          $(I)lines.hpp $(I)llong.hpp $(I)loader.hpp $(I)nalt.hpp   \
	          $(I)netnode.hpp $(I)pro.h $(I)range.hpp $(I)segment.hpp   \
	          $(I)ua.hpp $(I)xref.hpp \
	          src/loadmap.cpp src/MAPReader.cpp src/stdafx.cpp

BASE_OBJS += $(F)loadmap$(O) $(F)MAPReader$(O) $(F)stdafx$(O)

$(F)%$(O): src/%.cpp
	$(strip $(QCXX)$(CXX) $(CXXFLAGS) $(NORTTI) $(CONLY) $(OBJSW)$@ $<)
