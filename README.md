##Mushroom：非关系型数据库
[English Version of README](./README.en.md)

###蘑菇：基于 B link 树索引的非关系型数据库

####这是 [Up Database](http://www.github.com/UncP/Up_Database) 以及 [pear Database](http://www.github.com/UncP/pear) 的升级版本


###B link 树性能信息
`关键值长度： 16 字节`  
`关键值数量： 1000 万组`

|  版本号  |  线程数  |  排序时间（秒） |           改进             |
|--------|:-------:|:-----------:|:-----------------------------:|
| 0.1.0  |  单线程  |    16.00    ||
| 0.2.0  |  多线程  |    12.32    ||
| 0.2.1  |  多线程  |    11.28    |            锁管理器优化                      |
| 0.3.0  |  多线程  |    10.94    |  前缀压缩，索引占用内存减少约 9.1 %             |
| 0.4.0  |  多线程  |    11.44    |  二次映射多线程工作队列，减小程序整体内存超过 50 % |
| 0.4.1  |  多线程  |    1.01(1M)  |  修改并发协议，合并锁管理器与页面管理器，使每次插入操作减少1把锁 |


###TODO
- [x] B link 树索引管理器
- [x] 线程池
- [x] 多线程工作队列(有界，二次映射)
- [x] 锁管理器
- [x] 并发索引
- [ ] 数据管理器
