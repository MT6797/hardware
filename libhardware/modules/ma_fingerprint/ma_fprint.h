/* 
 * ma_fprint.h
 *
 *  Created on: 2016-3-5
 *      Author: czl
 * 		version: 2.4
 */

#ifndef MA_FPRINT_H_
#define MA_FPRINT_H_
#ifdef __cplusplus
extern "C" {
#endif

#define YUD_ACQUIRED

typedef uint8_t uint8;
typedef uint32_t uint32;
typedef uint64_t uint64;

enum fp_acquired_info {
	FINGERPRINT_ACQUIRED_ALI_BASE = 1100,
	FINGERPRINT_ACQUIRED_WAIT_FINGER_INPUT = FINGERPRINT_ACQUIRED_ALI_BASE + 1,
	FINGERPRINT_ACQUIRED_FINGER_DOWN = FINGERPRINT_ACQUIRED_ALI_BASE + 2,
	FINGERPRINT_ACQUIRED_FINGER_UP = FINGERPRINT_ACQUIRED_ALI_BASE + 3,
	FINGERPRINT_ACQUIRED_INPUT_TOO_LONG = FINGERPRINT_ACQUIRED_ALI_BASE + 4,
#ifdef YUD_ACQUIRED
	FINGERPRINT_ACQUIRED_DUPLICATE_FINGER = 2000,
#else
	FINGERPRINT_ACQUIRED_DUPLICATE_FINGER = FINGERPRINT_ACQUIRED_ALI_BASE + 5,
#endif
	FINGERPRINT_ACQUIRED_DUPLICATE_AREA = FINGERPRINT_ACQUIRED_ALI_BASE + 6,
	FINGERPRINT_ACQUIRED_LOW_COVER = FINGERPRINT_ACQUIRED_ALI_BASE + 7,
	FINGERPRINT_ACQUIRED_BAD_IMAGE = FINGERPRINT_ACQUIRED_ALI_BASE + 8,

	FINGERPRINT_ACQUIRED_MA_BASE = 1200,
	FINGERPRINT_ACQUIRED_TAP = FINGERPRINT_ACQUIRED_MA_BASE + 1,		/* tap */
	FINGERPRINT_ACQUIRED_DOUBLE_TAP = FINGERPRINT_ACQUIRED_MA_BASE + 2,	/* double tap */
	FINGERPRINT_ACQUIRED_LONG_PRESS = FINGERPRINT_ACQUIRED_MA_BASE + 3,	/* long press */
	FINGERPRINT_ACQUIRED_MOVE_UP = FINGERPRINT_ACQUIRED_MA_BASE + 4,	/* move up */
	FINGERPRINT_ACQUIRED_MOVE_DOWN = FINGERPRINT_ACQUIRED_MA_BASE + 5,	/* move down */
	FINGERPRINT_ACQUIRED_MOVE_LEFT = FINGERPRINT_ACQUIRED_MA_BASE + 6,	/* move left */
	FINGERPRINT_ACQUIRED_MOVE_RIGHT = FINGERPRINT_ACQUIRED_MA_BASE + 7,	/* move right */
};

/* 打开设备
 * @path 路径
 * @return
 * 		成功: >=0
 * 		设备打开失败: FP_FILE_FAIL
 * 		内存分配失败: FP_NOMEM
 * 		通讯失败: FP_COMM_FAIL
 * 		SQL执行失败: FP_SQL_FAIL
 */
extern int ma_open();

/* 关闭设备
 * @return
 * 		成功: FP_OK
 * 		关闭失败: FP_FILE_FAIL
 */
extern int ma_close();

/* pre_enroll
 * @return
 *		0 if function failed
 *  	otherwise, a uint64_t of token
 */
extern uint64 ma_pre_enroll();

/* enroll
 * @return
 * 		0 if enrollment process can be successfully started
 *     	or a negative number in case of error, generally from the errno.h set.
 *      A notify() function may be called indicating the error condition.
 */
extern int ma_enroll(const hw_auth_token_t *hat, int gid, int timeout);

/* post enroll
 * @return
 * 		0 if the request is accepted
 *      or a negative number in case of error, generally from the errno.h set.
 */
extern int ma_post_enroll();

/* get authenticator id
 * @return current authenticator id or 0 if function failed.
 */
extern uint64 ma_get_auth_id();

/* cancel
 * @return
 * 		0 if cancel request is accepted
 *      or a negative number in case of error, generally from the errno.h set.
 */
extern int ma_cancel();

/* remove finger
 * @gid userID
 * @fid fingerID
 * @return:
 * 		0 if fingerprint template(s) can be successfully deleted
 *      or a negative number in case of error, generally from the errno.h set.
 */
extern int ma_remove(int gid, int fid);

/* set group
 * @gid userID
 * @path store path
 * @return
 * 		0 on success
 *      or a negative number in case of error, generally from the errno.h set.
 */
extern int ma_set_active_group(int gid, const char *path);

/* authenticator
 *  @return
 *  	0 on success
 *      or a negative number in case of error, generally from the errno.h set.
 */
extern int ma_verify(uint64 operation_id, int gid);

/* set notify
 *  @return
 * 		0 if callback function is successfuly registered
 *      or a negative number in case of error, generally from the errno.h set.
 */
extern int ma_set_notify(fingerprint_notify_t notify);

#ifdef YUD_ACQUIRED
extern int ma_setNavMode(int nav);

extern int ma_getMode(void);

extern int ma_setTapTimeout(int timeout);

extern int ma_setDoubleTapMinTime(int min);

extern int ma_setDoubleTapTimeout(int timeout);

extern int ma_setLongPressTimeout(int timeout);
#endif

#ifdef __cplusplus
}
#endif
#endif /* MA_FPRINT_H_ */


