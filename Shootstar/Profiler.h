#pragma once

#include <Godot.hpp>
#include <Node.hpp>
#include <Timer.hpp>
#include <vector>
#include <map>
#include <algorithm>
#include <chrono>

namespace godot {
	using duration = std::chrono::microseconds;
	using time_point = std::chrono::high_resolution_clock::time_point;
	
	struct Profile {
		std::vector<duration> recorded_durations;
		time_point temp_start;
		time_point temp_end;
		int length;
		String name;
	};

	class Profiler : public Node
	{
	// Godot
	private:
		GODOT_CLASS(Profiler, Node)
	public:
		static void _register_methods();
		void _init();
		void _ready();
		void _process(float delta);

		Profiler();
		~Profiler();

	// Properties
	private:
		std::map<int, Profile> recorded_profiles;
		bool is_profiling = true;
		bool is_profile_once = true;
		bool is_report_brief = true;
		real_t profile_time = 30.0f;
		Timer* profile_timer;
	public:
		static Profiler *profiler_singleton;
	// Methods
	private:
		void on_timeout();
	public:
		void PrintDetail(Profile);
		void PrintSummary(Profile);
		void CreateNewProfile(int, String);
		void RecordStart(int);
		void RecordEnd(int);
	};
}


