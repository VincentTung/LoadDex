
## 动态加载Dex文件

### PathClassLoader DexClassLoader继承关系
 ![image](https://github-1304799125.cos.ap-beijing.myqcloud.com/LoadDex/img/loader_extends.jpg)
## 原理
###  读取dex文件生成一个新的DexClassLoader,然后利用反射将context.getClassLoader()的parent设置为新的DexClassLoader，利用了类加载机制中的双亲委派模型。
 ![image](https://github-1304799125.cos.ap-beijing.myqcloud.com/LoadDex/img/class_parents_%20delegate.png)

### context.getClassLoader()默认获得是一个PathClassLoader，它的parent是一个BootClassLoader
 ![image](https://github-1304799125.cos.ap-beijing.myqcloud.com/LoadDex/img/loader_debug.jpg)
 
### 通过反射机制改变的就是PathClassLoader的parent
 ![image](https://github-1304799125.cos.ap-beijing.myqcloud.com/LoadDex/img/loader_change_parent.jpg)

### dex文件生成过程
#### 1. class生成jar文件 
         jar -cvf demo.jar -C build/intermediates/classes/debug/ .
#### 2.  使用bulid-tools下的dx命令生成 dex文件
         ./dx --dex --output=classes.dex demo.jar
         
### dex加载流程(8.0)

 ![image](https://github-1304799125.cos.ap-beijing.myqcloud.com/LoadDex/img/dex_load_code_logic.png)
