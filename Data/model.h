#ifndef MODEL_H
#define MODEL_H

#include <vector>


struct PipeSection;
struct Pipeline;

enum class NodeType {
    End,
    Turn,
    CPS, /* Catode protection station */
    Protection,
    Material, /* Metall - Polyethene */
    CP, /* Control pipe */
    CCollector, /* Condence collector */
    CConductor, /* Control conductor */
    BValve, /* Ball valve */
    DFC, /* Dielectric flange connection */
    Valve, /* valve */
    GM, /* gas manhole */
    HL, /* hydraulic lock */
    Earthing /* earth electrode */
};

struct Node
{
    double x;
    double y;
    double z;

    NodeType type;

    PipeSection *section;
};


enum class PipeMaterial { Pe, St };

struct PipeSection
{
    PipeMaterial material;
    int diameter;

    std::vector<Node> nodes;

    Pipeline *pipeline;
};


enum class PressureType { G1, G2, G3, G4 };

struct Pipeline
{
    PressureType pressure;

    std::vector<PipeSection> sections;
};


struct Picket
{
    const Node* node;
};

class Model
{
public:
    void setData(std::vector<Pipeline> pipelines);
    int addPicket(const Picket& picket);

    const std::vector<Pipeline>& getPipelines() const;
    const std::vector<Node*>& getNodes() const;
    const std::vector<Picket>& getPickets() const;

private:
    std::vector<Pipeline> pipelines;
    std::vector<Node*> nodes;
    std::vector<Picket> pickets;
};

#endif // MODEL_H
