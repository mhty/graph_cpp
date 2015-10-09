//
//  graph.cpp
//  graph
//
//  Created by Dmitry Mikhaylovich on 09.10.15.
//  Copyright © 2015 Dmitry Mikhaylovich. All rights reserved.
//

#include "graph.hpp"



void graph::newDirectedEdge (Vertex* left, Vertex* right, weight_t weight) {
    left->edges.push_back(std::make_pair(right, weight));
};



void graph::newEdge (Vertex* left, Vertex* right, weight_t weight) {
    newDirectedEdge(left, right, weight);
    if (!directed) {
        newDirectedEdge(right, left, weight);
    }
};

void graph::deleteDirectedEdge (Vertex* left, Vertex* right) {
    for (__EdgeIterator E = left->edges.begin(); E != left->edges.end(); ++E) {
        if (E->first == right) {
            left->edges.erase(E);
        }
    }
};


void graph::deleteEdge (Vertex* left, Vertex* right) {
    deleteDirectedEdge(left, right);
    if (!directed) {
        deleteDirectedEdge(right, left);
    }
};

void graph::deleteOutway (Vertex* vertex) {
    vertex->edges.clear();
};


void graph::deleteInway (Vertex* vertex) {
    for (__VertexIterator V = vertexes.begin(); V != vertexes.end(); ++V) {
        deleteDirectedEdge(&(*V), vertex);
    }
};



graph::weight_t** graph::newMatrix () {
    size_t size = vertexesCount();
    weight_t** matrix = new weight_t*[size];
    for (int i = 0; i < size; ++i) {
        matrix[i] = new weight_t[size];
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = 0;
        }
    }
    return matrix;
};




graph::graph (bool _directed, bool _weighted): directed(_directed), weighted(_weighted) {};



graph::graph (size_t size, bool _directed, bool _weighted): directed(_directed), weighted(_weighted) {
    resize(size);
};

graph::~graph () {
    for (__VertexIterator V = vertexes.begin(); V != vertexes.end(); ++V) {
        V->edges.clear();
    }
    vertexes.clear();
};

size_t graph::vertexesCount () {
    return vertexes.size();
};


size_t graph::edgesCount () {
    size_t count = 0;
    for (__VertexIterator V = vertexes.begin(); V != vertexes.end(); ++V) {
        count += V->edges.size();
    }
    return count;
};



void graph::resize (size_t size) {
    vertexes.resize(size);
    for (int i = 0; i < size; ++i) {
        vertexes[i].data = i;
    }
};


void graph::newVertex () {
    int index = (int)vertexes.size();
    vertexes.push_back(Vertex(index));
};

void graph::newEdge (int left, int right, weight_t weight) {
    newEdge(&vertexes[left], &vertexes[right]);
};

void graph::deleteVertex (int index) {
    Vertex* vertex = &vertexes[index];
    deleteInway(vertex);
    deleteOutway(vertex);
    vertexes.erase(vertexes.begin() + index);
};

void graph::deleteEdge (int left, int right) {
    deleteEdge(&vertexes[left], &vertexes[right]);
};

void graph::asign (weight_t** matrix, size_t size) {
    resize(size);
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            if (matrix[y][x] > 0) {
                newEdge(y, x, matrix[y][x]);
            }
        }
    }
};

void graph::readFromFileAsMatrix (std::string filename) {
    std::ifstream fin(filename);
    size_t size;
    fin >> size;
    weight_t** matrix = new weight_t*[size];
    for (int i = 0; i < size; ++i) {
        matrix[i] = new weight_t[size];
    }
    
    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            fin >> matrix[y][x];
        }
    }
    
    asign(matrix, size);
    
    for (int i = 0; i < size; ++i) {
        delete []matrix[i];
    }
    delete []matrix;
    fin.close();
};

void graph::readFromFileAsEdgeList (std::string filename) {
    std::ifstream fin(filename);
    size_t size;
    fin >> size;
    int count;
    int left, right;
    fin >> count;
    resize(size);
    for (int i = 0; i < count; ++i) {
        fin >> left >> right;
        newEdge(left, right);
    }
    fin.close();
};

void graph::print () {
    for (__VertexIterator V = vertexes.begin(); V != vertexes.end(); ++V) {
        std::cout << V->data;
        for (__EdgeIterator E = V->edges.begin(); E != V->edges.end(); ++E) {
            std::cout << " ->" << E->first->data;
        }
        std::cout << std::endl;
    }
};



void graph::printAsMatrix () {
    size_t size = vertexesCount();
    weight_t** matrix = newMatrix();
    
    
    for (__VertexIterator V = vertexes.begin(); V != vertexes.end(); ++V) {
        for (__EdgeIterator E = V->edges.begin(); E != V->edges.end(); ++E) {
            matrix[V->data][E->first->data] = E->second;
        }
    }
    
    std::cout << size << std::endl;
    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            std::cout << matrix[y][x] << " ";
        }
        std::cout << std::endl;
    }
    
    
    for (int i = 0; i < size; ++i) {
        delete []matrix[i];
    }
    delete []matrix;
};



