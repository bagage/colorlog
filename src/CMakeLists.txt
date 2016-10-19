add_library(colorloglib
    CLHandler.cc
    Config.cc
    PipedChild.cc
    RuleBox.cc
    Rules.cc
    utils.cc
)
add_library(ColorlogCAPI colorlogCAPI.cc)
add_executable(colorlog main.cc)

target_link_libraries(colorlog colorloglib  ${Boost_LIBRARIES})
target_link_libraries(ColorlogCAPI colorloglib  ${Boost_LIBRARIES})

install(TARGETS colorlog DESTINATION bin)
