#ifndef __DVB_ENCODER_H_
#define __DVB_ENCODER_H_

#include <vector>

#include <lib/nav/core.h>
#include <lib/dvb/streamserver.h>

class eEncoder
{
	private:

		DECLARE_REF(eEncoder);

		enum
		{
			IOCTL_BROADCOM_SET_VPID_MIPS = 1,
			IOCTL_BROADCOM_SET_VPID_ARM = 11,
			IOCTL_BROADCOM_SET_APID_MIPS = 2,
			IOCTL_BROADCOM_SET_APID_ARM = 12,
			IOCTL_BROADCOM_SET_PMTPID_MIPS = 3,
			IOCTL_BROADCOM_SET_PMTPID_ARM = 13,
			IOCTL_BROADCOM_START_TRANSCODING = 100,
			IOCTL_BROADCOM_STOP_TRANSCODING = 200,
		};

		class EncoderContext : public eThread
		{
			public:

				EncoderContext(eNavigation *navigation_instance_normal_in, eNavigation *navigation_instance_alternative_in)
				{
					file_fd = -1;
					encoder_fd = -1;
					state = state_idle;
					navigation_instance = nullptr;
					navigation_instance_normal = navigation_instance_normal_in;
					navigation_instance_alternative = navigation_instance_alternative_in;
					stream_thread = nullptr;
				}

				int encoder_fd;
				int file_fd;
				eDVBRecordStreamThread *stream_thread;

				enum
				{
					state_idle,
					state_wait_pmt,
					state_running,
					state_finishing,
					state_destroyed,
				} state;

				eNavigation *navigation_instance;
				eNavigation *navigation_instance_normal;
				eNavigation *navigation_instance_alternative;

				void thread(void);
		};

		std::vector<EncoderContext> encoder;
		bool bcm_encoder;
		ePtr<eConnection> m_nav_event_connection_0;
		ePtr<eConnection> m_nav_event_connection_1;

		static eEncoder *instance;

		void navigation_event_0(int);
		void navigation_event_1(int);
		void navigation_event(int, int);

	public:

		eEncoder();
		~eEncoder();

		int allocateEncoder(const std::string &serviceref, int &buffersize, int bitrate, int width, int height, int framerate, int interlaced, int aspectratio,
				const std::string &vcodec = "", const std::string &acodec = "");
		int allocateHDMIEncoder(const std::string &serviceref, int &buffersize);
		void freeEncoder(int encoderfd);
		int getUsedEncoderCount();

		static eEncoder *getInstance();
};

#endif /* __DVB_ENCODER_H_ */
