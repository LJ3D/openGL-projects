#include "objVBO.h"



template<typename T> // This function is templated, but I dont think i actually need it to be lol. Whatever
void objVBO::readObjValues(const std::string fileName, const std::string startTag, const int dataCount, std::vector<T>& data){
    // Clear the data vector.
    data.clear();
    std::ifstream file(fileName);
    std::string line;
    while(std::getline(file, line)){ // 1: Read a line from the file
        if(line.substr(0,startTag.size()) == startTag){ // 2: Check if it starts with the startTag
            // 3: If so, break it down into <dataCount> ints and add them to the data vector:
            std::stringstream ss(line); 
            std::string token;
            std::getline(ss, token, ' '); // Skip the token at the beginning because it is not a number that we want.
            for(int i = 0; i < dataCount; i++){ // Read the rest of the tokens as ints.
                std::getline(ss, token, ' '); // Read the next token.
                T temp;
                std::from_chars(token.data(), token.data() + token.size(), temp);
                data.push_back(temp); // Push back may be slow, i think.
            }
        }
    }
}



// Read specifically the indices from the obj file. Use a struct to make it easier to use them later on.
void objVBO::readObjIndices(const std::string fileName, std::vector<vertexIndices>& data){
    data.clear();
    std::ifstream file(fileName);
    std::string line;
    while(std::getline(file, line)){ // 1: Read a line from the file
        if(line.substr(0,2) == "f "){ // 2: Check if it starts with the startTag
            // 3: If so, break it down into <dataCount> ints and add them to the data vector:
            std::stringstream ss(line); 
            std::string token;
            std::getline(ss, token, ' '); // Skip the token at the beginning because it is not a number that we want.
            for(int i = 0; i < 3; i++){ // Read the rest of the tokens as ints.
                std::getline(ss, token, ' '); // Read the next token.
                vertexIndices temp;
                std::stringstream ss2(token);
                std::getline(ss2, token, '/'); // Get the first token.
                std::from_chars(token.data(), token.data() + token.size(), temp.vertexIndex);
                std::getline(ss2, token, '/'); // Get the second token.
                std::from_chars(token.data(), token.data() + token.size(), temp.textureIndex);
                std::getline(ss2, token, '/'); // Get the third token.
                std::from_chars(token.data(), token.data() + token.size(), temp.normalIndex);
                data.push_back(temp); // Push back may be slow, i think.
            }
        }
    }
}






// =========================================================
//              OBJECT CLASS IMPLEMENTATION
// =========================================================



template<typename T>
void objVBO::object::printVector(const std::vector<T>& data){
    for(int i = 0; i < data.size(); i++){
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
}



objVBO::object::object(const std::string fileName){
    readObjValues(fileName, "v ", 3, positions);
    readObjValues(fileName, "vn", 3, normals);
    readObjValues(fileName, "vt", 2, textureCoords);
    readObjIndices(fileName, indices);
}



void objVBO::object::debugPrintData(){
    // Print position values
    std::cout << "Positions: " << std::endl;
    printVector(positions);
    // Print normal values
    std::cout << "Normals: " << std::endl;
    printVector(normals);
    // Print texture coordinate values
    std::cout << "Texture Coords: " << std::endl;
    printVector(textureCoords);
    std::cout << "==========================================================" << std::endl;
    // Print vertex indices
    std::cout << "Vertex Indices: " << std::endl;
    for(int i = 0; i < indices.size(); i++){
        std::cout << indices[i].vertexIndex << ", ";
        // New line every 3 indices.
        if(i % 3 == 2){
            std::cout << std::endl;
        }
    }
    std::cout << "Normal Indices: " << std::endl;
    for(int i = 0; i < indices.size(); i++){
        std::cout << indices[i].normalIndex << ", ";
        // New line every 3 indices.
        if(i % 3 == 2){
            std::cout << std::endl;
        }
    }
    std::cout << "Texture Indices: " << std::endl;
    for(int i = 0; i < indices.size(); i++){
        std::cout << indices[i].textureIndex << ", ";
        // New line every 3 indices.
        if(i % 3 == 2){
            std::cout << std::endl;
        }
    }
}

void objVBO::object::optimiseVBO(){
    // First break the data into a vector of vectors with size this->vertSize.
    std::vector<std::vector<float>> tempVertData;
    tempVertData.resize(this->VBO.size() / this->vertSize);
    for(int i = 0; i < tempVertData.size(); i++){
        tempVertData[i].resize(this->vertSize);
    }
    // Fill the new vector with the data.
    for(int i = 0; i < this->VBO.size(); i++){
        tempVertData[i / this->vertSize][i % this->vertSize] = this->VBO[i];
    }
    
    // Print duplicate std::vector<float>s in the tempVertData vector.
    // Using o(n^2) algorithm. (Bad)
    std::vector<unsigned int> duplicateIndices;
    for(int i = 0; i < tempVertData.size(); i++){
        for(int j = i + 1; j < tempVertData.size(); j++){
            if(tempVertData[i] == tempVertData[j]){
                std::cout << "Duplicate std::vector<float> found at index " << i << " and " << j << std::endl;
                duplicateIndices.push_back(i);
                duplicateIndices.push_back(j);
            }
        }
    }
    // Print the duplicate data:
    std::cout << "Duplicate std::vector<float> data: " << std::endl;
    for(int i = 0; i < duplicateIndices.size(); i++){
        std::cout << duplicateIndices[i] << ": ";
        for(int j = 0; j < tempVertData[duplicateIndices[i]].size(); j++){
            std::cout << tempVertData[duplicateIndices[i]][j] << " ";
        }
        std::cout << std::endl;
    }
}

void objVBO::object::assembleVBO(){
    // Check for the presence of position, normal, and texture coordinates:
    if(positions.size() == 0 || normals.size() == 0 || textureCoords.size() == 0){
        std::cout << "Error: Object does not have all required data:" << std::endl;
        // Figure out which data is missing.
        if(positions.size() == 0){
            std::cout << "Missing position data" << std::endl;
        }
        if(normals.size() == 0){
            std::cout << "Missing normal data" << std::endl;
        }
        if(textureCoords.size() == 0){
            std::cout << "Missing texture coordinate data" << std::endl;
        }
        // Throw runtime error
        throw std::runtime_error("Error: Object does not have all required data.");
    }

    // Create a VBO with all vertex data (expand the indices out to the full number of vertices)
    // Set indices to count up from 0 to the number of vertices. (This can be optimised later on)
    this->VBO.clear();
    this->vertSize = 3 + 3 + 2; // 3 floats for position, 3 floats for normal, 2 floats for texture coordinate. !!!!!! HARD CODE !!!!!!
    this->VBO.resize(vertSize * this->indices.size()); // Resize the VBO to the correct size.
    this->EBO.clear();
    this->EBO.resize(this->indices.size()); // Resize the EBO to the correct size.
    // !!!!!! HARD CODED - IDEALLY CAN ADAPT IF ANY ATTRIBUTE IS MISSING !!!!!!
    for(int i=0; i < this->VBO.size() / vertSize; i++){
        // Read position data from the indices.
        this->VBO[(i * vertSize) + 0] = positions[((this->indices[i].vertexIndex-1) * 3) + 0]; // The -1 is because the obj file indices start at 1, not 0. (Which is fucking retarded)
        this->VBO[(i * vertSize) + 1] = positions[((this->indices[i].vertexIndex-1) * 3) + 1];
        this->VBO[(i * vertSize) + 2] = positions[((this->indices[i].vertexIndex-1) * 3) + 2];
        // Read normal data
        this->VBO[(i * vertSize) + 3] = normals[((this->indices[i].normalIndex-1) * 3) + 0];
        this->VBO[(i * vertSize) + 4] = normals[((this->indices[i].normalIndex-1) * 3) + 1];
        this->VBO[(i * vertSize) + 5] = normals[((this->indices[i].normalIndex-1) * 3) + 2];
        // Read texCoord data
        this->VBO[(i * vertSize) + 6] = textureCoords[((this->indices[i].textureIndex-1) * 2) + 0];
        this->VBO[(i * vertSize) + 7] = textureCoords[((this->indices[i].textureIndex-1) * 2) + 1];
    }
    // Create the basic indices (just count up lol)
    for(int i = 0; i < this->VBO.size() / vertSize; i++){
        this->EBO[i] = i;
    }
}







void objVBO::object::writeVBO(const std::string filename){
    std::ofstream file;
    file.open(filename + ".vbo");
    // Write the entire VBO onto the first line of the file
    for(int i = 0; i < this->VBO.size(); i++){
        file << this->VBO[i] << " ";
    }
    file.close();
}

void objVBO::object::writeEBO(const std::string filename){
    std::ofstream file;
    file.open(filename + ".ebo");
    // Write the entire EBO onto the first line of the file
    for(int i = 0; i < this->EBO.size(); i++){
        file << this->EBO[i] << " ";
    }
    file.close();
}