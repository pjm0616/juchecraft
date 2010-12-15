
ADD_SUBDIRECTORY(actions)
ADD_SUBDIRECTORY(orders)

ADD_LIBRARY(mini_sc_game Exception.cpp Coordinate.cpp Object.cpp Player.cpp ObjectList.cpp UnitProductionManager.cpp ObjectFactory.cpp Target.cpp Game.cpp)
TARGET_LINK_LIBRARIES(mini_sc_game mini_sc_game_orders mini_sc_game_actions)

