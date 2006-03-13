INCLUDES = -I$(srcdir)/../kdeeduplot $(all_includes) -I$(srcdir)/../extdate

AM_CXXFLAGS = -DQT_PLUGIN
#KDE_CXXFLAGS = -UQT3_SUPPORT

METASOURCES = AUTO

kde_widget_LTLIBRARIES = kdeeduwidgets.la
kdeeduwidgets_la_LDFLAGS = $(KDE_PLUGIN) -module $(all_libraries)
kdeeduwidgets_la_LIBADD = $(LIB_KIO) ../kdeeduplot/libkdeeduplot.la ../extdate/libextdate.la
kdeeduwidgets_la_SOURCES = kdeeduwidgets.cpp

kdeeduwidgets.cpp: $(srcdir)/kdeedu.widgets
	$(MAKEKDEWIDGETS) -o kdeeduwidgets.cpp $(srcdir)/kdeedu.widgets

kdeeduwidgets.lo: kdeeduwidgets.moc
kdeeduwidgets.moc: kdeeduwidgets.cpp
	$(MOC) -o kdeeduwidgets.moc kdeeduwidgets.cpp

CLEANFILES = kdeeduwidgets.cpp kdeeduwidgets.moc
