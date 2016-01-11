

set(LIBISBN_SOURCE_FILES libisbn.cpp libisbn.hpp)

add_library(libisbn SHARED ${LIBISBN_SOURCE_FILES})

SET_TARGET_PROPERTIES(libisbn PROPERTIES PREFIX "")

install(TARGETS libisbn DESTINATION lib)
install(FILES libisbn.hpp DESTINATION include)

