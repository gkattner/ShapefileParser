// ShapefileParser.h: Includedatei für Include-Standardsystemdateien
// oder projektspezifische Includedateien.

#pragma once

#include "general.h"

#include <stdio.h>

#include <iostream>

//////////////// enums //////////////////////////
typedef enum {
	eReturnGlobalOk = 0,
	eReturnError = 1,
} ReturnCode_t;

//////////////// typedefs //////////////////////////
#pragma pack(push, 1)

typedef struct {
	double xmin;
	double ymin;
	double xmax;
	double ymax;
	double zmin;
	double zmax;
	double mmin;
	double mmax;
} HeaderBoundingBox_t;

typedef struct {
	uint32_t fileCode;
	uint32_t unused[5];
	uint32_t fileLength; // file length in 16 Bit - Words
	uint32_t version;
	uint32_t shapeType;
	HeaderBoundingBox_t boundingBox;
} FileHeader_t;

typedef struct {
	uint32_t recordNumber;
	uint32_t contentLength;
} RecordHeader_t;

typedef struct {
	double xmin;
	double xmax;
	double ymin;
	double ymax;
} Box_t;

typedef struct {
	uint64_t x;
	uint64_t y;
} Point_t;

typedef struct {
	uint32_t shapeType;
	Box_t box;
	uint32_t numParts;
	uint32_t numPoints;
	Parts_t* parts;
	Point_t* points;
} RecordPolyLineContents_t;

typedef struct {
	uint32_t numRecs;
	RecordHeader_t* rh;
	RecordPolyLineContents_t* rc;
} Record_t;

#pragma pack(pop)
