## Meta Data Object Model
The Object Model for the Metadata corresponds to the three json files
below. Basically all nodes (node types) in the json correcponds to one
class.

- Get Server Info
This describes the capabilities of the server, in our case the WASABI server.
WASABI will be a system type, different versions will expose more and more
capabilities

- Get MetaData
Here the metadata of a cube will be described. There will be a service class
that assembles the object model of this cube from a view in the sqllite catalog

- Get Response
This is the representation of a resultSet.