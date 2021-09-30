#include "pixel_neon_base.hpp"

#include "arithmetic/vadd.hpp"
#include "arithmetic/vaddl.hpp"
#include "arithmetic/vaddw.hpp"
#include "arithmetic/vaddhn.hpp"
#include "arithmetic/vqadd.hpp"
#include "arithmetic/vhadd.hpp"
#include "arithmetic/vrhadd.hpp"

#include "arithmetic/vsub.hpp"
#include "arithmetic/vsubl.hpp"
#include "arithmetic/vsubw.hpp"
#include "arithmetic/vsubhn.hpp"
#include "arithmetic/vqsub.hpp"
#include "arithmetic/vhsub.hpp"
#include "arithmetic/vrsubhn.hpp"

#include "multiply/vmul.hpp"
#include "multiply/vmul_n.hpp"
#include "multiply/vmul_lane.hpp"
#include "multiply/vmull.hpp"

#include "misc/vdup_n.hpp"