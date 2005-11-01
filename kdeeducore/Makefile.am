INCLUDES = $(all_includes)
# there's nothing in tests for the moment, so I'm removing it from the compilation
#SUBDIRS = . tests
SUBDIRS = .

lib_LTLIBRARIES = libkdeeducore.la

libkdeeducore_la_SOURCES = keduvocdata.cpp keduvocdocument.cpp \
	keduvocexpression.cpp keduvocgrammar.cpp keduvockvtmlreader.cpp keduvockvtmlwriter.cpp \
	keduvocmultiplechoice.cpp keduvowqlreader.cpp keduvowqlwriter.cpp leitnerbox.cpp \
	leitnersystem.cpp leitnersystemview.cpp prefleitner.cpp prefleitnerbase.ui
libkdeeducore_la_LDFLAGS = -no-undefined $(all_libraries) -version-info 3:0:2

libkdeeducore_includedir = $(includedir)/libkdeedu
libkdeeducore_include_HEADERS = keduvocdata.h keduvocdocument.h \
	keduvocexpression.h keduvocgrammar.h keduvockvtmlreader.h keduvockvtmlwriter.h \
	keduvocmultiplechoice.h keduvowqlreader.h keduvowqlwriter.h leitnerbox.h leitnersystem.h \
	leitnersystemview.h prefleitner.h

libkdeeducore_la_LIBADD = $(LIB_KDECORE) $(LIB_KFILE)

METASOURCES = AUTO

