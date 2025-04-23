cmake_minimum_required(VERSION 3.16.3)

project(dmenu)
set(STEST stest)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(X11 REQUIRED)
find_package(Freetype REQUIRED)
find_package(Fontconfig REQUIRED)

add_compile_definitions(VERSION="5.3")
include_directories(.)

add_executable(${PROJECT_NAME}
  dmenu.cpp
  drw.cpp
  util.cpp
)

target_link_libraries(${PROJECT_NAME}
  ${X11_LIBRARIES}
  ${X11_Xft_LIB}
  ${X11_Xinerama_LIB}
  ${FREETYPE_LIBRARIES}
  ${Fontconfig_LIBRARIES}
)
target_include_directories(${PROJECT_NAME}
  PRIVATE
  ${X11_INCLUDE_DIR}
  ${FREETYPE_INCLUDE_DIRS}
  ${Fontconfig_INCLUDE_DIRS}
)

add_executable(${STEST}
  stest.cpp
)

if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(googletest)
  add_subdirectory(tests)
endif()

# install: all
# 	mkdir -p $(DESTDIR)$(PREFIX)/bin
# 	cp -f dmenu dmenu_path dmenu_run stest $(DESTDIR)$(PREFIX)/bin
# 	chmod 755 $(DESTDIR)$(PREFIX)/bin/dmenu
# 	chmod 755 $(DESTDIR)$(PREFIX)/bin/dmenu_path
# 	chmod 755 $(DESTDIR)$(PREFIX)/bin/dmenu_run
# 	chmod 755 $(DESTDIR)$(PREFIX)/bin/stest
# 	mkdir -p $(DESTDIR)$(MANPREFIX)/man1
# 	sed "s/VERSION/$(VERSION)/g" < dmenu.1 > $(DESTDIR)$(MANPREFIX)/man1/dmenu.1
# 	sed "s/VERSION/$(VERSION)/g" < stest.1 > $(DESTDIR)$(MANPREFIX)/man1/stest.1
# 	chmod 644 $(DESTDIR)$(MANPREFIX)/man1/dmenu.1
# 	chmod 644 $(DESTDIR)$(MANPREFIX)/man1/stest.1
#
# uninstall:
# 	rm -f $(DESTDIR)$(PREFIX)/bin/dmenu\
# 		$(DESTDIR)$(PREFIX)/bin/dmenu_path\
# 		$(DESTDIR)$(PREFIX)/bin/dmenu_run\
# 		$(DESTDIR)$(PREFIX)/bin/stest\
# 		$(DESTDIR)$(MANPREFIX)/man1/dmenu.1\
# 		$(DESTDIR)$(MANPREFIX)/man1/stest.1
