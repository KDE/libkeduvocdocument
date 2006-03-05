kde4_header()

add_subdirectory( kdeeducore ) 
add_subdirectory( kdeeduui ) 
add_subdirectory( kdeeduplot ) 
add_subdirectory( extdate ) 
add_subdirectory( libscience ) 
add_subdirectory( widgets ) 

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############


kde4_create_doxygen_docs(  )

kde4_footer()



#original Makefile.am contents follow:

#SUBDIRS = kdeeducore kdeeduui kdeeduplot extdate libscience widgets
#
#messages:
#	$(XGETTEXT) `find . -name \*.h -o -name \*.cpp -o -name \*.cc` -o $(podir)/libkdeedu.pot 
#
#include ../admin/Doxyfile.am
