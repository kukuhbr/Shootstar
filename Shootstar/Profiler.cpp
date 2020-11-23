#include "Profiler.h"
#include "Commons.h"

godot::Profiler* Profiler::profiler_singleton;

void Profiler::_register_methods() {
	register_method((char*)"_ready", &Profiler::_ready);
	register_method((char*)"_process", &Profiler::_process);
	register_method((char*)"on_timeout", &Profiler::on_timeout);
	register_property((char*)"is_profiling", &Profiler::is_profiling, true);
	register_property((char*)"is_profile_once", &Profiler::is_profile_once, true);
	register_property((char*)"is_report_brief", &Profiler::is_report_brief, true);
	register_property((char*)"profile_time", &Profiler::profile_time, 30.0f);
}

void Profiler::_init() {}

void Profiler::_ready() {
	Profiler::profiler_singleton = this;
	profile_timer = Timer::_new();
	profile_timer->connect("timeout", this, "on_timeout");
	profile_timer->set_wait_time(profile_time);
	profile_timer->set_one_shot(is_profile_once);
	add_child(profile_timer);
	profile_timer->start();
}

void Profiler::_process(float delta) {}

void Profiler::on_timeout() {
	for (auto it = recorded_profiles.begin(); it != recorded_profiles.end(); ++it) {
		if (is_report_brief) {
			PrintSummary(it->second);
		}
		else {
			PrintDetail(it->second);
		}
	}
	
}

Profiler::Profiler() {
}

Profiler::~Profiler() {

}

void Profiler::PrintDetail(Profile prof) {
	Godot::print("Reporting for {0}", prof.name);
	duration min, max;
	duration sum = chrono::microseconds::zero();
	int count = 0;;
	for (auto iter = prof.recorded_durations.begin(); iter != prof.recorded_durations.end(); ++iter) {
		if (iter == prof.recorded_durations.begin()) {
			min = *iter;
			max = *iter;
		}
		if (*iter < min) {
			max = *iter;
		}
		if (*iter > max) {
			min = *iter;
		}
		sum += *iter;
		count++;
		Godot::print("Call {0} is {1} long", count, iter->count());
	}
	Godot::print("Average is {0} from {1} calls", sum.count() / count, count);
	Godot::print("Maximum is {0}", max.count());
	Godot::print("Minimum is {0}", min.count());
}

void Profiler::PrintSummary(Profile prof) {
	Godot::print("Reporting for {0}", prof.name);
	duration min, max;
	duration sum = chrono::microseconds::zero();
	int count = 0;;
	for (auto iter = prof.recorded_durations.begin(); iter != prof.recorded_durations.end(); ++iter) {
		if (iter == prof.recorded_durations.begin()) {
			min = *iter;
			max = *iter;
		}
		if (*iter < min) {
			max = *iter;
		}
		if (*iter > max) {
			min = *iter;
		}
		sum += *iter;
		count++;
	}
	Godot::print("Average is {0} from {1} calls", sum.count() / count, count);
	Godot::print("Maximum is {0}", max.count());
	Godot::print("Minimum is {0}", min.count());
}

void Profiler::CreateNewProfile(int id, String name) {
	Profile prof = {};
	prof.name = name;
	recorded_profiles.insert({ id, prof });
}

void Profiler::RecordStart(int id) {
	auto it = recorded_profiles.find(id);
	if (it != recorded_profiles.end()) {
		it->second.temp_start = chrono::high_resolution_clock::now();
	}
}

void Profiler::RecordEnd(int id) {
	auto it = recorded_profiles.find(id);
	if (it != recorded_profiles.end()) {
		it->second.temp_end = chrono::high_resolution_clock::now();
		duration record_duration = chrono::duration_cast<chrono::microseconds>(it->second.temp_end - it->second.temp_start);
		it->second.recorded_durations.push_back(record_duration);
	}
}