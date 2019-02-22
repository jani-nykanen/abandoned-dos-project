// Converts tmx-file to a binary format
// (c) 2019 Jani Nykänen

// ------------------------------- // 
// Declaration

#include <vector>
#include <string>
#include <iostream>


typedef std::vector<int> Layer;

// Key-value pair
struct KeyValuePair {
    std::string key;
    std::string value;

    // Constructors
    inline KeyValuePair() {

        this->key = "";
        this->value = "";
    }
    inline KeyValuePair(std::string key, std::string value) {

        this->key = key;
        this->value = value;
    }
};

// Tilemap type
class Tilemap {

private:

    // Data
    std::vector<Layer> layers;
    // Dimensions
    int width;
    int height;

    // Properties
    std::vector<KeyValuePair> properties;

public:

    // Constructor
    inline Tilemap() {}
    Tilemap(std::string path);

    // Get dimensions
    inline int getWidth(){return width;}
    inline int getHeight(){return height;}

    // Get a tile
    int getTile(int layer, int x, int y);
    // Get a property
    std::string getProp(std::string name);

    // Convert to a binary format
    void convertToBin(const char* out);
};


// ------------------------------- // 
// Definitions

#include <fstream>
#include <stdexcept>
#include <cstdio>
#include <sstream>


// Find parameter value
static std::string findValue(std::string content, 
    std::string what) {

    std::string lookFor = what + "=\"";
    int p = content.find(lookFor) + lookFor.length();
    // Find the next quotation mark
    int i = p;
    for(; i < content.length(); ++ i) {

        if(content[i] == '"') {
            break;
        }
    }

    return content.substr(p,i-p);
}


// Parse properties
static void parseProperties(std::string content,
    std::vector<KeyValuePair> &properties) {

    // Find all the property positions
    std::vector<int> propPos;
    std::vector<int> propEnds;

    int p = 0;
    int oldP = 0;
    // Find property positions
    std::string find = "<property";
    while(true) {

        oldP = p;
        p = content.find(find, p+find.length());
        if(p <= oldP)
            break;

        // Push
        propPos.push_back(p + find.length());
    }

    // Find property keys & values
    // TODO: Inefficient with big data!
    std::string key, value;
    for(int i = 0; i < propPos.size(); ++ i) {

        key = findValue(content.substr(propPos[i]), "name");
        value = findValue(content.substr(propPos[i]), "value");

        // Store properties
        properties.push_back(KeyValuePair(key, value));
    }
}


// Parse CSV (to integers)
static void parseCSVInt(std::string content, 
    std::vector<int> &data) {

    std::string buffer = "";
    int count = 0;
    char c;
    for(int i = 0; i < content.size(); ++ i) {

        c = content[i];
        if(buffer.length() > 0 && c == ',') {

            if(count < data.size()) {
                
                std::istringstream(buffer) >> data[count ++];
                buffer = "";
            }
        }
        // Push to the buffer if not a whitespace
        else if(!(c == ' ' || c == '\n' || c == '\t')) {

            buffer.push_back(content[i]);
        }
    }
    if(buffer.length() > 0) {

         std::istringstream(buffer) >> data[count ++];
    }
}


// Parse layers
static void parseLayers(std::string content, 
    std::vector<Layer> &layers, int size) {

    // Find all the layer positions
    std::vector<int> start;
    std::vector<int> end;

    int p1 = 0;
    int p2 = 0;
    int oldP = 0;
    // Find layer positions
    std::string findBegin = "<data encoding=\"csv\">";
    std::string findEnd = "</data>";
    int count = 0;
    while(true) {

        oldP = p1;
        p1 = content.find(findBegin, p1 + findBegin.length()) 
            + findBegin.length();
        p2 = content.find(findEnd, p2 + findEnd.length());

        if(p1 <= oldP)
            break;

        // Push
        start.push_back(p1);
        end.push_back(p2);

        ++ count;
    }

    // Read CSV data
    for(int i = 0; i < count; ++ i) {
        
        // Add a new layer
        layers.push_back(Layer(size));
        // Parse data to the layer
        parseCSVInt(content.substr(start[i], 
            end[i]-start[i]),
            layers[i]);
    }
}


// Constructor
Tilemap::Tilemap(std::string path) {

    // Read file to a string
    std::string content = "";
    std::ifstream file(path.c_str());
    if(file.is_open() == false) {

        throw std::runtime_error("Failed to open a file in " + path);
    }

    // Read lines
    std::string line;
    while (std::getline(file, line)) {

        content += line;
    }

    // Find dimensions
    std::istringstream(findValue(content,"width")) >> width;
    std::istringstream(findValue(content,"height")) >> height;

    // Parse properties
    properties = std::vector<KeyValuePair> ();
    parseProperties(content, properties);

    // Parse map data
    std::string findBegin = "<data encoding=\"csv\">";
    std::string findEnd = "</data>";
    int begin = content.find(findBegin) + findBegin.length();
    int end = content.find(findEnd);

    // Parse layers
    parseLayers(content, layers, width*height);
}


// Get tile
int Tilemap::getTile(int layer, int x, int y) {

    if(layer < 0 || layer >= layers.size() || 
       x < 0 || y < 0 || x >= width || y >= height)
        return -1;

    return layers[layer] [y*width +x];
}


// Get a property
std::string Tilemap::getProp(std::string name) {

    for(int i = 0; i < properties.size(); ++ i) {

        if(properties[i].key == name) {

            return properties[i].value;
        }
    }
    return "0";
}


// Convert to a binary format
void Tilemap::convertToBin(const char* out) {

    FILE* f = fopen(out, "wb");
    if(f == NULL) {

        throw std::runtime_error(
            "Failed to create the output file:" + std::string(out) + "\n");
    }

    // Size
    unsigned short w = width;
    unsigned short h = height;
    fwrite(&w, sizeof(short), 1, f);
    fwrite(&h, sizeof(short), 1, f);

    // Layer count
    unsigned char layerCount = (unsigned char)layers.size();
    fwrite(&layerCount, sizeof(char), 1, f);

    // Copy data and store it
    std::vector<unsigned char> data;
    for(int i = 0; i < (int)layers.size(); ++ i) {

        // Copy data
        for(int j = 0; j < width*height; ++ j) {

            data.push_back((unsigned char)layers[i][j]);
        }
    }
    // Store
    fwrite(&data[0], sizeof(char), width*height*layerCount, f);

    fclose(f);
}


// ----------------------------- //
// Main

int main(int argc, char** argv) {

    const int REQ_ARGS = 3;
    if(argc < REQ_ARGS) {

        printf("Need more arguments. Help: ./tmx2bin src dst\n");
        return 0;
    }

    // Load a tilemap
    Tilemap* t;
    try {
        t = new Tilemap(std::string(argv[1]));
    }
    catch(std::runtime_error err) {

        printf("%s\n", err.what());
    }

    // Convert to binary
    t->convertToBin(argv[2]);

    delete t;

    return 0;
}
