#pragma once

#include "math/matrix_t.hpp"
#include "math/vector2d.hpp"
#include "misc/color.hpp"
#include "classes/c_usercmd.hpp"
#include "classes/recv_props.hpp"
#include "classes/client_class.hpp"
#include "classes/convar.hpp"
#include "classes/studio.hpp"
#include "structs/dlight.hpp"
#include "structs/weaponinfo.hpp"
#include "classes/view_setup.h"
#include "classes/entities.hpp"
#include "structs/animstate.hpp"
#include <array>

static constexpr std::array sky_list = {
			"Default",
			"cs_baggage_skybox_",
			"cs_tibet",
			"vietnam",
			"sky_lunacy",
			"embassy",
			"italy",
			"jungle",
			"office",
			"sky_cs15_daylight01_hdr",
			"sky_cs15_daylight02_hdr",
			"sky_day02_05",
			"nukeblank",
			"dustblank",
			"sky_venice",
			"sky_cs15_daylight03_hdr",
			"sky_cs15_daylight04_hdr",
			"sky_csgo_cloudy01",
			"sky_csgo_night02",
			"sky_csgo_night02b",
			"vertigo",
			"vertigoblue_hdr",
			"sky_dust",
			"sky_day02_05_hdr"
};