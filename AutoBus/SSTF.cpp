#include "AutoBus.h"

int flag_passrequest = 0, flag_target = 0;

int AutoBus::sstf_station()
{
	return state.position / config.distance;
}

int AutoBus::sstf_find_flag() // �ҳ����·��
{
	int distance = 1, request = 1, count_request = 1, count_distance = -1;
	int tip = 0, tip_count = 0;
	// �ҳ�˳ʱ�������µ������Ŀ��ľ���
	while (1) {
		if (sstf_station() + distance >= config.total_station) {
			distance = -(sstf_station());
		}
		if (distance == 0) {
			break;
		}
		if (state.clockwise_request[sstf_station() + distance + 1]
			|| state.counterclockwise_request[sstf_station() + distance + 1]
			|| state.target[sstf_station() + distance + 1]) {
			tip = 1;
			break;
		}
		else if (distance != 0) {
			distance++;
			request++;
		}
	}
	// �ҳ���ʱ�������µ������Ŀ��վ��ľ���
	while (1) {
		if (sstf_station() + count_distance <= -1) {
			count_distance = -(sstf_station() - config.total_station + 1);
		}
		if (count_distance == 0) {
			break;
		}
		if (state.clockwise_request[sstf_station() + count_distance + 1]
			|| state.counterclockwise_request[sstf_station() + count_distance + 1]
			|| state.target[sstf_station() + count_distance + 1]) {
			tip_count = 1;
			break;
		}
		else if (count_distance != 0) {
			count_distance--;
			count_request++;
		}
	}

	if (tip == 1 && tip_count == 1 && count_request < request) {
		return (-count_request);
	}
	if (tip == 1 && tip_count == 1 && count_request >= request) {
		return request;
	}
	if (tip == 0) {
		return 0;
	}
}

void AutoBus::sstf_state_change() // �ı�position��˳ʱ������£�
{
	if (state.position == 0) {
		state.position = config.distance * config.total_station - 1;
	}
	else {
		state.position--;
	}
}

void AutoBus::sstf_state_change_target() // ȷ����Ŀ�겢����state.current_targetȷ�����з���
{
	if (state.current_target == 0) {
		state.current_target = sstf_find_flag();
	}
	if (state.current_target > 0) {
		state.state = 3;
	}
	else if (state.current_target < 0) {
		state.state = 1;
	}
	else if (state.current_target == 0) {
		state.state = 0;
	}
}

void AutoBus::sstf_state_change_target_new() // ��λ��վ��ʱ���ı�target���������״̬
{
	if (state.state == 1) {
		state.current_target++;
	}
	else if (state.state == 3) {
		state.current_target--;
	}
	if ((state.state == 1
		&& (state.target[sstf_station() + 1] ||
			state.counterclockwise_request[sstf_station() + 1]))
		|| state.current_target == 0) {
		state.state = 2;
		flag_passrequest = 1;

		if (state.current_target == 0) {
			flag_target = 1;
		}
	}
	else if ((state.state == 3
		&& (state.target[sstf_station() + 1] ||
			state.clockwise_request[sstf_station() + 1]))
		|| state.current_target == 0) {
		state.state = 2;
		flag_passrequest = 3;

		if (state.current_target == 0) {
			flag_target = 1;
		}
	}

}

void AutoBus::sstf_clock_tick()
{
	state.time++;
	// ������һ��Ϊpark������һ��֮ǰ��������ʱ�ж���һ������״̬
	if (state.state == 0 && state.current_target != 0) {
		if (state.current_target > 0) {
			state.state = 3;
		}
		else if (state.current_target < 0) {
			state.state = 1;
		}
	}
	state.last_state = state.state;
	if (state.state == 3) {
		if (state.position != (config.distance * config.total_station) - 1) {
			state.position++;
		}
		else {
			state.position = 0;
		}
	}
	else if (state.state == 1) {
		sstf_state_change();
	}
	else if (state.state == 2) {
		sstf_state_change_target();
	}
	if (state.position % config.distance == 0 && state.last_state != 2
		&& state.last_state != 0) {
		sstf_state_change_target_new();
	}
	if (state.position % config.distance == 0 && state.last_state == 2) {
		if (flag_target == 1) {
			state.counterclockwise_request[sstf_station() + 1] = 0;
			state.target[sstf_station() + 1] = 0;
			state.clockwise_request[sstf_station() + 1] = 0;
		}
		else {
			if (flag_passrequest == 1) {
				state.target[sstf_station() + 1] = 0;
				state.counterclockwise_request[sstf_station() + 1] = 0;
			}
			else if (flag_passrequest == 3) {
				state.target[sstf_station() + 1] = 0;
				state.clockwise_request[sstf_station() + 1] = 0;
			}
		}
		flag_target = 0;
		flag_passrequest = 0;
	}
	if (state.position % config.distance == 0 && state.state == 0) {
		if (state.target[sstf_station() + 1] == 1) {
			state.target[sstf_station() + 1] = 0;
		}
		if (state.clockwise_request[sstf_station() + 1] == 1) {
			state.clockwise_request[sstf_station() + 1] = 0;
		}
		if (state.counterclockwise_request[sstf_station() + 1] == 1) {
			state.counterclockwise_request[sstf_station() + 1] = 0;
		}
	}
}
void AutoBus::sstf_primary_request(int direction, int station)
{
	if (direction == 1) {
		state.clockwise_request[station] = 1;
	}
	else if (direction == -1) {
		state.counterclockwise_request[station] = 1;
	}
	if (state.state == 0) {
		state.current_target = sstf_find_flag();
	}
}

void AutoBus::sstf_secondary_request(int target)
{
	state.target[target] = 1;
	if (state.state == 0) {
		state.current_target = sstf_find_flag();
	}
}
