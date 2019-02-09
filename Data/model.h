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

class Model
{
public:
    Model();

    const std::vector<Node>& getNodes() const;

private:
    std::vector<Node> nodes;
};

#endif // MODEL_H
