kde4_header()
project(libkdeedu)

add_subdirectory(kdeeducore)
add_subdirectory(kdeeduui)
add_subdirectory(kdeeduplot)
add_subdirectory(extdate)
add_subdirectory(libscience)
add_subdirectory(widgets)

kde4_footer()

#original Makefile.am contents follow:

#SUBDIRS = kdeeducore kdeeduui kdeeduplot extdate libscience widgets
#
#messages:
#	$(XGETTEXT) `find . -name \*.h -o -name \*.cpp -o -name \*.cc` -o $(podir)/libkdeedu.pot 
#
#include ../admin/Doxyfile.am
