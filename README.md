
## 动态加载Dex文件

## 原理
###  读取dex文件生成一个新的DexClassLoader,然后利用反射将context.getClassLoade()的parent设置为新的DexClassLoader，利用了类加载机制中的双亲委派模型。
 ![image](https://github.com/VincentTung/LoadDex/blob/master/img/class_parents_%20delegate.png)
### dex文件生成过程
#### 1. class生成jar文件 
         jar -cvf demo.jar -C build/intermediates/classes/debug/ .
#### 2.  使用bulid-tools下的dx命令生成 dex文件
         ./dx --dex --output=classes.dex demo.jar
