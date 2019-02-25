AUX_SOURCE_DIRECTORY(. SOURCE_LIST)
ADD_EXECUTABLE(jt808c ${SOURCE_LIST})
TARGET_LINK_LIBRARIES(jt808c pthread)
INCLUDE_DIRECTORIES(
../../ql-ol-extsdk/include/ 
/home/iasuser/workspace/T-BOX/T-BOX2/TPE_4G_STD/open-ec20/ql-ol-crosstool/sysroots/armv7a-vfp-neon-oe-linux-gnueabi/usr/include/qmi/
/home/iasuser/workspace/T-BOX/T-BOX2/TPE_4G_STD/open-ec20/ql-ol-crosstool/sysroots/armv7a-vfp-neon-oe-linux-gnueabi/usr/include/qmi-framework
)

install(TARGETS jt808c DESTINATION ${target_dir})
