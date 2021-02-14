[使用OpenCV中的universal intrinsics为算法提速 (1)](https://mp.weixin.qq.com/s?__biz=MjM5NTE3NjY5MA==&mid=2247484025&idx=1&sn=132d0fc0a242df11bd5b59cd22eaad99&chksm=a6fdcbe4918a42f2dae5c150541b8e243bfe4d70e126813852c0d8b108d37af28e9713a4699e&scene=21#wechat_redirect)

[使用OpenCV中的universal intrinsics为算法提速 (2)](https://mp.weixin.qq.com/s?__biz=MjM5NTE3NjY5MA==&mid=2247484072&idx=1&sn=e04b079225776cfde7c400d319f58448&chksm=a6fdcb35918a4223ce56f1b79359c0159000ec0b5d674cb7e820aaa6d5a1010f4ba2fe022ccc&scene=21#wechat_redirect)

[使用OpenCV中的universal intrinsics为算法提速 (3)](https://mp.weixin.qq.com/s?src=11&timestamp=1613301941&ver=2890&signature=NacoD9gG2TtepdxxvAzYPATCU-fW7DHosaJFqEg7u0fXcAUiVwnhjScV43Zkf*NI-rr5nLI0e2l7epFcz3g4Gv*VnnPosSTTtLt0kR2cdlhf8wqSN7Jq8DbJDLLKah3x&new=1)

## no -mavx2
CV_SIMD is : 1
CV_SIMD_WIDTH is : 16
CV_SIMD128 is : 1
CV_SIMD256 is : 0
CV_SIMD512 is : 0
sizeof(v_uint8) = 16
sizeof(v_int32) = 16
sizeof(v_float32) = 16
time cost for random assign is 2348.799711 ms
impl1, result is -5290.376953, time cost is 213.656658 ms
step is 4
impl2, result is -5289.430176, time cost is 68.728935 ms


## -mavx2

CV_SIMD is : 1
CV_SIMD_WIDTH is : 32
CV_SIMD128 is : 1
CV_SIMD256 is : 1
CV_SIMD512 is : 0
sizeof(v_uint8) = 32
sizeof(v_int32) = 32
sizeof(v_float32) = 32
time cost for random assign is 2214.837345 ms
impl1, result is -5290.376953, time cost is 215.468339 ms
step is 8
impl2, result is -5290.986328, time cost is 61.731527 ms

# android armv8
CV_SIMD is : 1
CV_SIMD_WIDTH is : 16
CV_SIMD128 is : 1
CV_SIMD256 is : 0
CV_SIMD512 is : 0
sizeof(v_uint8) = 16
sizeof(v_int32) = 16
sizeof(v_float32) = 16
time cost for random assign is 2231.197447 ms
impl1, result is -5290.376953, time cost is 346.908750 ms
step is 4
impl2, result is -5289.429688, time cost is 191.786354 ms
