# First Server

## 说明
使用 C++ 语言，通过面相对象的方式开发的服务端。

### 特点
+ 底层数据的粘包问题已经处理，通过 onReceive() 方法接收到的数据已经处理了分包/合包问题
+ 最大数据包限制，超过 Buff::MAX_LENGTH 的包自动丢弃，服务端主动断开客户端连接
+ 心跳检测机制（todo）
+ 