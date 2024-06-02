// ShapefileParser.cpp: Definiert den Einstiegspunkt für die Anwendung.
//

#include "ShapefileParser.h"
#include "general.h"

#include <fstream>

using namespace std;

int length;
char* buffer;

//////////////// function declarations //////////////////////////
Return_t ParseRecordHeader(RecordHeader_t* fh);

//////////////// function definitions //////////////////////////
Return_t ParseRecordHeader(RecordHeader_t* fh) {
	return eReturnGlobalOk;
}

int main()
{
	cout << "Parsing shapefile.\n" << endl;

	ifstream is;
	is.open("C:\\Users\\gkat\\source\\repos\\ShapefileParser\\data\\ne_110m_coastline\\ne_110m_coastline.shp", ios::binary);

	// get length of file:
	is.seekg(0, ios::end);
	length = is.tellg();
	is.seekg(0, ios::beg);
	// allocate memory:
	buffer = new char[length];
	// read data as a block:
	is.read(buffer, length);
	is.close();

	FileHeader_t fh;
	memcpy(&fh, buffer, sizeof(FileHeader_t));
	fh.fileCode = little2native32(fh.fileCode);
	fh.fileLength = little2native32(fh.fileLength);

	Record_t r;
	RecordHeader_t rh;
	uint32_t currentPositionInFile = sizeof(FileHeader_t);
	uint32_t cnt = 0;
	uint32_t ptr = 0;

	uint32_t numRecs = 0;

	// get number of records in file
	do {
		memcpy(&rh, (buffer + currentPositionInFile), sizeof(RecordHeader_t));
		rh.contentLength = little2native32(rh.contentLength);
		currentPositionInFile += sizeof(RecordHeader_t);
		currentPositionInFile += (rh.contentLength * 2);
		numRecs++;
	} while (currentPositionInFile < (fh.fileLength * 2));

	r.numRecs = numRecs;
	r.rh = (RecordHeader_t*)malloc(numRecs * sizeof(RecordHeader_t));
	r.rc = (RecordPolyLineContents_t*)malloc(numRecs * sizeof(RecordPolyLineContents_t));


	currentPositionInFile = sizeof(FileHeader_t);

	for (uint32_t cnt = 0; cnt < r.numRecs; cnt++) {
		memcpy((&r.rh[cnt]), (buffer + currentPositionInFile), sizeof(RecordHeader_t));

		r.rh[cnt].recordNumber;

		r.rh[cnt].recordNumber = little2native32(r.rh[cnt].recordNumber);
		r.rh[cnt].contentLength = little2native32(r.rh[cnt].contentLength);

		currentPositionInFile += sizeof(RecordHeader_t);		
		
		RecordPolyLineContents_t rc;
		Parts_t* parts = new Parts_t;
		Point_t* points = new Point_t;
		uint32_t cpySize;

		uint32_t add;

		add = currentPositionInFile + 4 + sizeof(Box_t);
		r.rc[cnt].numParts = *(buffer + add);
		add = currentPositionInFile + 8 + sizeof(Box_t);		
		memcpy(&r.rc[cnt].numPoints, (buffer + add), sizeof(uint32_t));
		//r.rc[cnt].numPoints = *(buffer + add);

		cpySize = r.rc[cnt].numParts * sizeof(uint32_t);
		parts = (Parts_t*)malloc(cpySize);
		r.rc[cnt].parts = (Parts_t*)malloc(cpySize);
		ptr = currentPositionInFile + 12 + sizeof(Box_t);
		for (int i = 0; i < r.rc[cnt].numParts; i++) {
			//parts[i] = *(buffer + ptr + (4 * i));
			r.rc[cnt].parts[i] = *(buffer + ptr + (4 * i));
		}		

		cpySize = r.rc[cnt].numPoints * sizeof(Point_t);
		points = (Point_t*)malloc(cpySize);
		r.rc[cnt].points = (Point_t*)malloc(cpySize);
		ptr = currentPositionInFile + 12 + sizeof(Box_t) + (r.rc[cnt].numParts * sizeof(uint32_t));
		for (int i = 0; i < r.rc[cnt].numPoints; i++) {			
			//memcpy(&points[i].x, (buffer + ptr + (i * (sizeof(Point_t)))), 8);
			memcpy(&r.rc[cnt].points[i].x, (buffer + ptr + (i * (sizeof(Point_t)))), 8);
			//memcpy(&points[i].y, (buffer + ptr + (i * (sizeof(Point_t))) + 8), 8);
			memcpy(&r.rc[cnt].points[i].y, (buffer + ptr + (i * (sizeof(Point_t))) + 8), 8);
		}

		//memcpy(r.rc[cnt].parts, parts, r.rc[cnt].numParts * sizeof(uint32_t));
		memcpy(r.rc[cnt].points, points, r.rc[cnt].numPoints * sizeof(Point_t));

		parts = NULL;
		points = NULL;

		currentPositionInFile += (r.rh[cnt].contentLength * 2);
		
	}

	return eReturnGlobalOk;
}


