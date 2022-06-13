#include <string>
#include <iostream>
#include "mosquittopp.h"

class mqtt_test : public mosqpp::mosquittopp
{
public:
    mqtt_test ( const char* id ) : mosquittopp ( id ) {}
    void on_connect ( int rc )
    {
        std::cout << "on_connect" << std::endl;
    }
    void on_disconnect()
    {
        std::cout << "on_disconnect" << std::endl;
    }
    void on_publish ( int mid )
    {
        std::cout << "on_publish" << std::endl;
    }
    void on_subscribe ( int mid, int qos_count, const int* granted_qos ); //订阅回调函数
    void on_message ( const struct mosquitto_message* message ); //订阅主题接收到消息
};
std::string g_subTopic = "subTopic";
void mqtt_test::on_subscribe ( int mid, int qos_count, const int* granted_qos )
{
    std::cout << "订阅 mid: %d " << mid << std::endl;
}
void mqtt_test::on_message ( const struct mosquitto_message* message )
{
    bool res = false;
    mosqpp::topic_matches_sub ( g_subTopic.c_str(), message->topic, &res );
    if ( res ) {
        std::string strRcv = ( char* ) message->payload;
        std::cout << "来自<" << message->topic << ">的消息：" << strRcv << std::endl;
    }
}
int main ( int argc, char* argv[] )
{
    mosqpp::lib_init();
    mqtt_test test ( "client6" );

    int rc;
    char buf[1024] = "This is test";
    test.username_pw_set ( "wmy", "mqtt" );
    rc = test.connect ( "127.0.0.1" ); //本地IP
    char err[1024];
    if ( rc == MOSQ_ERR_ERRNO ) {
        std::cout << "连接错误:" << mosqpp::strerror ( rc ) << std::endl;    //连接出错
    } else if ( MOSQ_ERR_SUCCESS == rc ) {
        //发布测试
        rc = test.loop();
        if ( MOSQ_ERR_SUCCESS == rc ) {
            rc = test.publish ( NULL, "topic/test", strlen ( buf ), ( const void* ) buf );
            rc = test.loop();
        }
        // rc = test.disconnect(); //订阅测试时注释该行
        //  rc = test.loop(); //订阅测试时注释该行

        test.subscribe ( NULL, g_subTopic.c_str() ); //订阅测试取消注释该行
        rc = test.loop_forever();//订阅测试取消注释该行
    }
    mosqpp::lib_cleanup();
    system ( "pause" );
    return 0;
}
