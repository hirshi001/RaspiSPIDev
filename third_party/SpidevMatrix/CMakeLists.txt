

# Ensure spidev-lib++ is available (only fetch if missing)
if(NOT TARGET SpidevMatrix)
    FetchContent_Declare(
            SpidevMatrix
            GIT_REPOSITORY https://github.com/hirshi001/SpidevMatrix.git
            GIT_TAG master
    )
    FetchContent_MakeAvailable(SpidevMatrix)
endif()
