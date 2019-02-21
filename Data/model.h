#ifndef MODEL_H
#define MODEL_H

#include <vector>


enum class NodeType { End, Turn };

struct Node
{
    double x;
    double y;
    double z;

    NodeType type;
};

struct Picket
{
    const Node* node;
};

class Model
{
public:
    Model();

    size_t addPicket(const Picket& picket);

    const std::vector<Node>& getNodes() const;
    const std::vector<Picket>& getPickets() const;

private:
    std::vector<Node> nodes;
    std::vector<Picket> pickets;
};

#endif // MODEL_H
