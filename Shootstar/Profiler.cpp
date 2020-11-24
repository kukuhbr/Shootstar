#include "Profiler.h"
#include "Commons.h"

godot::Profiler* Profiler::profiler_singleton;

void Profiler::_register_methods() {
	register_method((char*)"_ready", &Profiler::_ready);
	register_method((char*)"_process", &Profiler::_process);
	register_method((char*)"on_timeout", &Profiler::on_timeout);
	register_property((char*)"is_profiling", &Profiler::is_profiling, true);
	register_property((char*)"is_profile_once", &Profiler::is_profile_once, false);
	register_property((char*)"is_report_brief", &Profiler::is_report_brief, true);
	register_property((char*)"profile_time", &Profiler::profile_time, 30.0f);
}

void Profiler::_init() {}

void Profiler::_ready() {
	if (is_profiling) {
		Profiler::profiler_singleton = this;
		profile_timer = Timer::_new();
		profile_timer->connect("timeout", this, "on_timeout");
		profile_timer->set_wait_time(profile_time);
		profile_timer->set_one_shot(is_profile_once);
		add_child(profile_timer);
		profile_timer->start();
	}
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
	duration sum = duration::zero();
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
	Godot::print("Average is {0} microseconds from {1} calls", sum.count() / count, count);
	Godot::print("Maximum is {0} microseconds", max.count());
	Godot::print("Minimum is {0} microseconds", min.count());
}

void Profiler::PrintSummary(Profile prof) {
	Godot::print("Reporting for {0}", prof.name);
	duration min, max;
	duration sum = duration::zero();
	int count = 0;;
	for (auto iter = prof.recorded_durations.begin(); iter != prof.recorded_durations.end(); ++iter) {
		if (iter == prof.recorded_durations.begin()) {
			min = *iter;
			max = *iter;
		}
		if (*iter < min) {
			min = *iter;
		}
		if (*iter > max) {
			max = *iter;
		}
		sum += *iter;
		count++;
	}
	Godot::print("Average is {0} microseconds from {1} calls", sum.count() / count, count);
	Godot::print("Maximum is {0} microseconds", max.count());
	Godot::print("Minimum is {0} microseconds", min.count());
}

void Profiler::CreateNewProfile(int id, String name) {
	auto it = recorded_profiles.find(id);
	if (it == recorded_profiles.end()) {
		Profile prof = {};
		prof.name = name;
		recorded_profiles.insert({ id, prof });
		Godot::print("Profile for {0} created at id {1}", name, id);
	}
}

time_point Profiler::Record(int id) {
	auto it = recorded_profiles.find(id);
	if (it != recorded_profiles.end()) {
		return chrono::high_resolution_clock::now();
	}
}

void Profiler::RecordTime(int id, time_point end, time_point start) {
	auto it = recorded_profiles.find(id);
	if (it != recorded_profiles.end()) {
		duration record_duration = chrono::duration_cast<duration>(end - start);
		it->second.recorded_durations.push_back(record_duration);
	}
}