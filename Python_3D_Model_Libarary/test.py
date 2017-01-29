from parsingModule import getVolume
from parsingModule import getArea
from parsingModule import getNumFacets
from parsingModule import getAll
print getVolume("robot.stl")
print getArea("robot.stl")
print getNumFacets("test.stl")
tub = getAll("robot.stl")
print tub