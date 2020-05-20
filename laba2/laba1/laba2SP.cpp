#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "windows.h"
#include <fstream>
#include <string>

using namespace std;

struct record {
	unsigned char index;
	FILETIME creationTime;
	char content[80];
	unsigned char accessCount;
};

string SDate(unsigned short day, unsigned short month, unsigned short year) {
	string rs = std::to_string(day) + "th of "s;
	switch (month)
	{
	case 1:
		rs += "January";
		break;
	case 2:
		rs += "February";
		break;
	case 3:
		rs += "March";
		break;
	case 4:
		rs += "April";
		break;
	case 5:
		rs += "May";
		break;
	case 6:
		rs += "June";
		break;
	case 7:
		rs += "July";
		break;
	case 8:
		rs += "August";
		break;
	case 9:
		rs += "September";
		break;
	case 10:
		rs += "October";
		break;
	case 11:
		rs += "November";
		break;
	case 12:
		rs += "December";
		break;
	default:
		rs += "???";
		break;
	}
	rs += " "s + std::to_string(year);
	return rs;
}
string STime(unsigned short hour, unsigned short minute, unsigned short second) {
	return std::string(std::to_string(hour) + ":"s + std::to_string(minute) + ":"s + std::to_string(second));
}
#pragma region UserCases
void cs_create_file() {
	unsigned short FileSize;
	unsigned char FRecordCount;

	SYSTEMTIME system_time;
	FILETIME file_time;

	FILE *file = fopen("RecordList", "w");
	cout << "Enter number of records: ";
	scanf("%hhu", &FRecordCount);
	unsigned char b = '0';
	fwrite(&b, sizeof(char), 1, file);
	fwrite(&FileSize, sizeof(short), 1, file);
	for (unsigned char i = 0; i < (int)FRecordCount; i++)
	{
		GetSystemTime(&system_time);
		SystemTimeToFileTime(&system_time, &file_time);
		struct record rec = { 1, file_time,"",0 };
		rec.index = i;
		fwrite(&rec, sizeof(struct record), 1, file);
	}
	fseek(file, 0, SEEK_END);
	FileSize = ftell(file);
	fseek(file, sizeof(char), SEEK_SET);
	fwrite(&FileSize, sizeof(short), 1, file);
	fclose(file);
	cout << "A file was created.";
}
bool cs_change_file(record *inp_rec, unsigned char *ind) {

	FILE *file = fopen("RecordList", "r+");
	SYSTEMTIME local_time;

	bool search_end_trigger = false;
	cout << "Enter record's Index to access it: ";
	scanf("%hhu", ind);
	fseek(file, sizeof(char) + sizeof(short), SEEK_SET);
	while (!search_end_trigger && fread(inp_rec, sizeof(struct record), 1, file))
	{
		if (inp_rec->index == *ind) {
			search_end_trigger = true;
			cout << "Requested record was found.\n";
			if ((string)inp_rec->content == "") {
				cout << "No content was found\n";
			}
			else {
				cout << "Content: " << inp_rec->content << "\n";
			}

			FileTimeToSystemTime(&(inp_rec->creationTime), &local_time);
			cout << "Last modified: " << SDate(local_time.wDay, local_time.wMonth, local_time.wYear) << " ";
			cout << STime(local_time.wHour, local_time.wMinute, local_time.wSecond) << endl;
			cout << "Number of modifications: " << (int)inp_rec->accessCount << endl;
		}
	}
	fclose(file);
	return search_end_trigger;
}
void cs_delete_record(record inp_rec, unsigned char ind) {
	FILE *file = fopen("RecordList", "r+");
	SYSTEMTIME system_time;
	FILETIME file_time;

	if ((string)inp_rec.content != "") {
		unsigned char b = '0';
		fseek(file, 0, SEEK_SET);
		fread(&b, sizeof(char), 1, file);
		b -= 1;
		fseek(file, 0, SEEK_SET);
		fwrite(&b, sizeof(char), 1, file);
	}
	fseek(file, sizeof(char) + sizeof(short) + (int)ind * sizeof(struct record), SEEK_SET);
	GetSystemTime(&system_time);
	SystemTimeToFileTime(&system_time, &file_time);
	struct record empty = { ind, file_time,"",0 };
	fwrite(&empty, sizeof(struct record), 1, file);
	cout << "Record was deleted.\n";
	fclose(file);
}
void cs_change_record(record inp_rec, unsigned char ind) {
	FILE *file = fopen("RecordList", "r+");
	char cont[80];
	cout << "Input new content: ";
	cin.getline(cont, sizeof(cont));
	cin.getline(cont, sizeof(cont));
	cin.clear();
	cin.ignore(INT_MAX, '\n');
	if ((string)cont != "") {
		unsigned char b = '0';
		fseek(file, 0, SEEK_SET);
		fread(&b, sizeof(char), 1, file);
		b += 1;
		fseek(file, 0, SEEK_SET);
		fwrite(&b, sizeof(char), 1, file);
	}
	strncpy(inp_rec.content, cont, sizeof(inp_rec.content));
	inp_rec.accessCount++;
	fseek(file, sizeof(char) + sizeof(short) + (int)ind * sizeof(struct record), SEEK_SET);
	fwrite(&inp_rec, sizeof(struct record), 1, file);
	fclose(file);
}
#pragma endregion UserCases
int main()
{
#pragma region variables
	enum program_mode { cs_exit, create_file, change_file };
	enum record_mode { rcd_exit, delete_record, change_record };
	int mode;
	bool program_exit_trigger;

#pragma endregion variables
	program_exit_trigger = false;
	do {
		cout << "\nChoose work mode:\n\n1 - create new file;\n2 - change previous one;\n0 - close program.\n";
		cin >> mode;
		switch (mode)
		{
		case cs_exit: {
			program_exit_trigger = true;
			break;
		}
		case create_file: {
			cs_create_file();
			break;
		}
		case change_file: {
			record inp_rec;
			unsigned char ind;
			bool found_record_trigger = cs_change_file(&inp_rec, &ind);
			if (found_record_trigger) {
				int rcd_mode;
				cout << "Choose operation: 1 - delete record; 2 - change it's content; 0 - cancel operation;\n";
				cin >> rcd_mode;
				switch (rcd_mode)
				{
				case rcd_exit: {
					break;
				}
				case delete_record: {
					cs_delete_record(inp_rec, ind);
					break;
				}
				case change_record: {
					cs_change_record(inp_rec, ind);
					break;
				}
				default:
					cout << "No such operation.\n";
					break;
				}
			}
			else {
				cout << "A record with given Index doesn't exist.\n";
			}
			break;
		}
		default:
			cout << "No such operation. Try again.\n";
			break;
		}
	} while (!program_exit_trigger);
}