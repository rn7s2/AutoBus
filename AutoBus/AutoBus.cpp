#include "AutoBus.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <wx/msgdlg.h>

AutoBus::AutoBus()
{
	Init();
	ReadConfig();
}

AutoBus::~AutoBus()
{
	list_free(state.requests);
}

void AutoBus::Dispatch(const Instruction& t)
{
	switch (config.strategy) {
	case FCFS:
		DispatchFCFS(t);
		break;
	case SSTF:
		DispatchSSTF(t);
		break;
	case SCAN:
		DispatchSCAN(t);
		break;
	default:
		fprintf(stderr, "Unknown strategy!\n");
		exit(-1);
	}
}

void AutoBus::Init()
{
	state.last_state = -1;
	state.state = 0;
	state.current_target = 0;
	state.time = state.position = 0;
	state.requests = list_node_new(-1, -1);
	memset(state.target, 0, sizeof(state.target));
	memset(state.clockwise_request, 0, sizeof(state.clockwise_request));
	memset(state.counterclockwise_request, 0,
		sizeof(state.counterclockwise_request));

	config.total_station = 5;
	config.strategy = FCFS;
	config.distance = 2;
}

void AutoBus::ReadConfig()
{
	FILE* fin = fopen("dict.dic", "r");
	char buf[MAX_BUF] = {};

	if (fin == nullptr) {
		wxMessageBox(_("Configuration file not found."));
		return;
	}

	while (fgets(buf, MAX_BUF, fin)) {
		char* right = nullptr;

		int tmp = static_cast<int>(strlen(buf) - 1);
		while (buf[tmp] == ' ' || buf[tmp] == '\n') {
			buf[tmp] = '\0', --tmp;
		}

		switch (buf[0]) {
		case 'T':
			right = strrchr(buf, ' '), right++;
			config.total_station = atoi(right);
			break;
		case 'S':
			right = strrchr(buf, ' '), right++;
			if (!strcmp(right, "FCFS")) {
				config.strategy = FCFS;
			}
			else if (!strcmp(right, "SSTF")) {
				config.strategy = SSTF;
			}
			else {
				config.strategy = SCAN;
			}
			break;
		case 'D':
			right = strrchr(buf, ' '), right++;
			config.distance = atoi(right);
			break;
		default:
			continue;
		}
	}

	fclose(fin);
}

void AutoBus::DispatchFCFS(const Instruction& t)
{
	switch (t.type) {
	case 1:
		fcfs_clock_tick();
		break;
	case 2:
		fcfs_primary_request(-1, t.station);
		break;
	case 3:
		fcfs_primary_request(1, t.station);
		break;
	case 4:
		fcfs_secondary_request(t.station);
		break;
	default:
		fprintf(stderr, "Unknown command!\n");
		exit(-1);
	}
}

void AutoBus::DispatchSSTF(const Instruction& t)
{
	switch (t.type) {
	case 1:
		sstf_clock_tick();
		break;
	case 2:
		sstf_primary_request(-1, t.station);
		break;
	case 3:
		sstf_primary_request(1, t.station);
		break;
	case 4:
		sstf_secondary_request(t.station);
		break;
	default:
		fprintf(stderr, "Unknown command!\n");
		exit(-1);
	}
}

void AutoBus::DispatchSCAN(const Instruction& t)
{
	switch (t.type) {
	case 1:
		scan_clock_tick();
		break;
	case 2:
		scan_primary_request(-1, t.station);
		break;
	case 3:
		scan_primary_request(1, t.station);
		break;
	case 4:
		scan_secondary_request(t.station);
		break;
	default:
		fprintf(stderr, "Unknown command!\n");
		exit(-1);
	}
}
