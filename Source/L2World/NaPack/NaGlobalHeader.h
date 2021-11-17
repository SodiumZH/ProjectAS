/* By Sodium */
/**

	This is the global header of all NaPacks.
	Any usage of Na-packs needs to correctly set this header and place it at the parent folder of NaPacks.

*/

#pragma once
// Match NaPack-API to the project's API.
#define NAPACK_API L2WORLD_API
#define NAPACK_ENABLE_DEBUG true

// Module config. If a module is not loaded, set the corresponding value false.
#define NAPACK_MOB_SYSTEM true
#define NAPACK_ITEM_SYSTEM true
#define NAPACK_UI_SYSTEM true

// Debug settings
#define SAFE_ASSERT_ON true	// If true, the assertion macros "checks" "checksf" will work.