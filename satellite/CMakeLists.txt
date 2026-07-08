cmake_minimum_required(VERSION 3.16)

project(asteria_satellite C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(asteria_core
    src/commands.c
    src/event_log.c
    src/health.c
    src/mission.c
    src/scheduler.c
    src/storage.c
    src/subsystems.c
    src/telemetry.c
    src/drivers/adc.c
    src/drivers/radio.c
    src/drivers/sensor.c
)

target_include_directories(asteria_core PUBLIC include)

if(MSVC)
    target_compile_options(asteria_core PRIVATE /W4)
else()
    target_compile_options(asteria_core PRIVATE -Wall -Wextra -pedantic)
endif()

add_executable(asteria-sim src/main.c)
target_link_libraries(asteria-sim PRIVATE asteria_core)

add_executable(asteria-tests tests/test_mission.c)
target_link_libraries(asteria-tests PRIVATE asteria_core)

enable_testing()
add_test(NAME mission_tests COMMAND asteria-tests)
