#include "LifeView.h"
#include <string>

void write_file_to_file(FILE *src, FILE *dest){
	if(src != NULL && dest != NULL){
		char c;
		while( (c=fgetc(src)) != EOF){
			fputc(c, dest);
		}
	}
}

void append_file_to_file(string src, FILE *dest_file){
	FILE *source_file = fopen(src.c_str(), "r");
	write_file_to_file(source_file, dest_file);
}


BasicCmdLineView::BasicCmdLineView(SimOptions *options, SimController *contr){
	opts = options;
	controller = contr;
}

void VisualOutput::writeout(FILE* out){
	for(int y = opts->windowY.getHigh(); y >= opts->windowY.getLow(); y--){
		for(int x = opts->windowX.getLow(); x <= opts->windowX.getHigh(); x++){
			fprintf(out, "%c", opts->getCharforState(controller->getStateOfCoord(x,y)));
		}
		fprintf(out, "\n");
	}
}

void FileFormatOutput::writeout(FILE* out){
	vector<string> states = opts->getValidIdentifiers();
	fprintf(out, "%s = \n{\n", opts->getSimType().c_str());
	fprintf(out, "\tName = \"%s\";\n", opts->name.getString().c_str());
	fprintf(out, "\tTerrain = {\n\t\tXrange = %d..%d;\n\t\tYrange = %d..%d;\n\t};\n", opts->terrainX.getLow(), opts->terrainX.getHigh(), opts->terrainY.getLow(), opts->terrainY.getHigh());
	fprintf(out, "\tWindow = {\n\t\tXrange = %d..%d;\n\t\tYrange = %d..%d;\n\t};\n", opts->windowX.getLow(), opts->windowX.getHigh(), opts->windowY.getLow(), opts->windowY.getHigh());
	fprintf(out, "\tChars = {\n");
	for(int i = 0; i < states.size(); i++){
			fprintf(out, "\t\t%s = %d;\n", states.at(i).c_str(), opts->getCharforState(states.at(i)));
	}
	fprintf(out, "\t};\n");
	fprintf(out, "\tColors = {\n");
	for(int i = 0; i < states.size(); i++){
			fprintf(out, "\t\t%s = (%d, %d, %d);\n", states.at(i).c_str(), opts->getColorForState(states.at(i)).getFirst(), opts->getColorForState(states.at(i)).getSecond(), opts->getColorForState(states.at(i)).getThird());
	}
	fprintf(out, "\t};\n");

	fprintf(out, "\tInitial = {\n");
	for(int i = 0; i < states.size(); i++){
		if(states.at(i) != opts->getDefaultStateString()){
			fprintf(out, "\t\t%s = ", states.at(i).c_str());
			const vector<Pair> *list = controller->getPairListForState(states.at(i))->getPairVector();
			for(int i = 0; i < list->size(); i++){
				fprintf(out, " (%d, %d)", list->at(i).getFirst(), list->at(i).getSecond());
				if(i < list->size()-1){
					fprintf(out, ",");
				}
			}
			fprintf(out, ";\n");
		}
	}
	fprintf(out, "\t};\n");
	fprintf(out, "};\n");
}

void LifeHelpOutput::writeout(FILE *out){
	append_file_to_file(string("LifeHelp.txt"), out);
}