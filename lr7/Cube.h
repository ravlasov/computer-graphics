#include <vector>
#include <iostream>
#include <ctgmath>

class Cube{
public:
    Cube(int n){
	std::vector<std::vector<float>> sections;
	std::vector<std::vector<float>> neg_sections;
	for(float z=-1.0+2.0/n; z<1.0; z+=2.0/n) {
	    std::cout << 1-pow(z, 6) << std::endl;
	    if(1-pow(z, 6) <= pow(0.55, 6))
		continue;
	    sections.push_back(std::vector<float>());
	    neg_sections.push_back(std::vector<float>());
            for(float y=-1.0; y<=1.0; y+=2.0/n) {
                float x = pow(1-pow(z, 6)-pow(y, 6), 1.0/6);
		if(!std::isnan(x)){
		    sections.back().push_back(x);
		    sections.back().push_back(y);
		    sections.back().push_back(z);
		    neg_sections.back().push_back(-x);
		    neg_sections.back().push_back(y);
		    neg_sections.back().push_back(z);
		    float nx, ny, nz;
		    nx = 2;
		    ny = y + pow(y/x, 5)*(nx-x);
		    nz = z + pow(z/x, 5)*(nx-x);
		    sections.back().push_back(nx);
		    sections.back().push_back(ny);
		    sections.back().push_back(nz);
		    neg_sections.back().push_back(-nx);
		    neg_sections.back().push_back(ny);
		    neg_sections.back().push_back(nz);
		    sections.back().push_back((z+1)/2);
		    sections.back().push_back((y+1)/2);
		    neg_sections.back().push_back((z+1)/2);
		    neg_sections.back().push_back((y+1)/2);
		}
	    }
	    float y = sections.back()[sections.back().size()-7];

	    int size = neg_sections.back().size();
	    for(int i=size-1; i>=0; i-=8){
		for(int j=7; j>=0; --j)
		    sections.back().push_back(neg_sections.back()[i-j]);
	    }
	    for(int i=size; i<neg_sections.back().size(); ++i)
		sections.back().push_back(neg_sections.back()[i]);

	}
	float last = sections.size()-1;
	for(int i=0; i<sections[last].size()/16; ++i){
            sections.push_back(std::vector<float>());
	    for(int j=0; j<8; ++j){
	        sections.back().push_back(sections[last][i*8+j]);
	    }
	    for(int j=7; j>=0; --j){
	        sections.back().push_back(sections[last][sections[last].size()-i*8-j-1]);
	    }
	}
	for(int i=0; i<sections[0].size()/16; ++i){
            sections.push_back(std::vector<float>());
	    for(int j=0; j<8; ++j){
	        sections.back().push_back(sections[0][i*8+j]);
	    }
	    if(sections[0][i*8+1] < 0.57 && sections[0][i*8+1] > -0.57){
	        sections.back().push_back(0.5);
	        for(int j=1; j<8; ++j){
	            sections.back().push_back(sections[0][i*8+j]);
	        }
                sections.push_back(std::vector<float>());
	        sections.back().push_back(0.11);
	        for(int j=1; j<8; ++j){
	            sections.back().push_back(sections[0][i*8+j]);
	        }
	        sections.back().push_back(-0.11);
	        for(int j=1; j<8; ++j){
	            sections.back().push_back(sections[0][i*8+j]);
	        }
                sections.push_back(std::vector<float>());
	        sections.back().push_back(-0.5);
	        for(int j=1; j<8; ++j){
	            sections.back().push_back(sections[0][i*8+j]);
	        }
	    }
	    for(int j=7; j>=0; --j){
	        sections.back().push_back(sections[0][sections[0].size()-i*8-j-1]);
	    }
	}
	std::cout << sections.size() << std::endl;
	std::cout << sections[0].size() << std::endl;
	unsigned int prev_levels = sections[0].size()/8;
	for(int s=1; s<sections.size(); ++s){
	    for(unsigned int i=1; i<sections[s].size()/8; ++i){
	        indices.push_back(prev_levels+i-1);
	        indices.push_back(prev_levels+i);
	    }
	    indices.push_back(prev_levels+sections[s].size()/8-1);
	    indices.push_back(prev_levels);
	    prev_levels += sections[s].size()/8;
	}
        for(int s=0; s<sections.size(); ++s)
	    points.insert(points.end(), sections[s].begin(), sections[s].end());

	/*for(int i=0; i<sections[60].size(); ++i){
	    if(i % 8 < 3)
                std::cout << sections[60][i] << ' ';
	    if((i+1) % 8 == 0)
	        std::cout << std::endl;
	}*/
	/*for(int i=1; i<sections.size(); ++i){
            std::cout << sections[i][0] << ' ' << sections[i][1] << ' ' << sections[i][2] << std::endl;
	}*/
    }
    std::vector<float> get_points(){
	return points;
    }
    std::vector<float> get_normals(){
	return normals;
    }
    std::vector<float> get_tex_coords(){
	return texCoords;
    }
    std::vector<unsigned int> get_indices(){
	return indices;
    }
    std::vector<float> get_full(){
	return points;
    }

private:
    std::vector<float> points;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<unsigned int> indices;
};
