# Add project specific ProGuard rules here.
# By default, the flags in this file are appended to flags specified
# in D:\adt-bundle-windows-x86-20131030\sdk/tools/proguard/proguard-android.txt
# You can edit the include path and order by changing the proguardFiles
# directive in build.gradle.
#
# For more details, see
#   http://developer.android.com/guide/developing/tools/proguard.html

# Add any project specific keep options here:

# If your project uses WebView with JS, uncomment the following
# and specify the fully qualified class name to the JavaScript interface
# class:
#-keepclassmembers class fqcn.of.javascript.interface.for.webview {
#   public *;
#}

# 指定代码的压缩级别，默认已设置
-optimizationpasses 5
# 是否使用大小写混合，默认已设置
-dontusemixedcaseclassnames
# 指定不去忽略非公共的库类，默认已设置
-dontskipnonpubliclibraryclasses
# 指定不去忽略包可见的库类的成员
-dontskipnonpubliclibraryclassmembers
# 混淆时所采用的算法
-optimizations !code/simplification/arithmetic,!field/*,!class/merging/*
# 混淆时是否优化，默认已设置
-dontoptimize
# 混淆时是否做预校验
-dontpreverify
# 屏蔽警告
-ignorewarnings
# 混淆时是否记录日志，默认的
-verbose
# 保留原始的类名和行号
-keepattributes SourceFile,LineNumberTable


# 保持标记和签名不被混淆
-keepattributes *Annotation*
-keep class * extends java.lang.annotation.Annotation { *; }
-keepattributes Signature

#事件总线注解
-keep @com.squareup.otto.Subscribe class * {*;}
-keep class * {
    @com.squareup.otto.Subscribe <fields>;
}
-keepclassmembers class * {
    @com.squareup.otto.Subscribe <methods>;
}
-keep @com.squareup.otto.Produce class * {*;}
-keep class * {
    @com.squareup.otto.Produce <fields>;
}
-keepclassmembers class * {
    @com.squareup.otto.Produce <methods>;
}


# 保持类不被混淆
-keep class * extends android.app.Fragment
-keep class * extends android.app.Activity
-keep class * extends android.app.Application
-keep class * extends android.app.Service
-keep class * extends android.content.BroadcastReceiver
-keep class * extends android.content.ContentProvider
-keep class * extends android.app.backup.BackupAgentHelper
-keep class * extends android.preference.Preference
-keep class * extends android.support.v4.**
-keep class com.android.vending.licensing.ILicensingService

-keep class android.support.** {*;}
-dontwarn android.support.**

# 保持R文件不被混淆：
-keep class **.R$* {*;}

# 保持 native 方法不被混淆
-keepclasseswithmembernames class * {
    native <methods>;
}

# 保持自定义控件类不被混淆
-keepclasseswithmembers class * {
    public <init>(android.content.Context, android.util.AttributeSet);
}
-keepclasseswithmembers class * {
    public <init>(android.content.Context, android.util.AttributeSet, int);
}
-keepclassmembers class * extends android.app.Activity {
   public void *(android.view.View);
}

# 保持枚举 enum 类不被混淆
-keepclassmembers enum * {
    public static **[] values();
    public static ** valueOf(java.lang.String);
}

# 保持 Parcelable 不被混淆
-keep public class * implements android.os.Parcelable {
  public static final android.os.Parcelable$Creator *;
}
-keepnames class * implements java.io.Serializable



# 保持序列化类不被混淆
-keepclassmembers class * implements java.io.Serializable {
   static final long serialVersionUID;
   private static final java.io.ObjectStreamField[] serialPersistentFields;
   !static !transient <fields>;
   private void writeObject(java.io.ObjectOutputStream);
   private void readObject(java.io.ObjectInputStream);
   java.lang.Object writeReplace();
   java.lang.Object readResolve();
}


-keep public class com.alibaba.android.arouter.routes.**{*;}
-keep class * implements com.alibaba.android.arouter.facade.template.ISyringe{*;}