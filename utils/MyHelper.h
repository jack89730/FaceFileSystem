#ifndef MYHELPER
#define MYHELPER
#include <QtCore>
#include <QtGui>
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
#include <QtWidgets>

#endif
#include <QDateTime>
#include "widgets/MyInputBox.h"
#include "widgets/MyMessageBox.h"


class myHelper: public QObject
{
public:
    //设置为开机启动
    static void AutoRunWithSystem(bool IsAutoRun, QString AppName, QString AppPath) {
        QSettings *reg = new QSettings(
            "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
            QSettings::NativeFormat);

        if (IsAutoRun) {
            reg->setValue(AppName, AppPath);
        } else {
            reg->setValue(AppName, "");
        }
    }



    //设置编码为UTF8
    static void SetUTF8Code() {
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
        QTextCodec *codec = QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForLocale(codec);
        QTextCodec::setCodecForCStrings(codec);
        QTextCodec::setCodecForTr(codec);
#endif
    }

    //设置为中文字符
    static void SetChinese() {
        QTranslator *translator = new QTranslator(qApp);
        translator->load(":/image/qt_zh_CN.qm");
        qApp->installTranslator(translator);
    }

    //设置指定样式
    static void SetStyle(const QString &qssFile) {
        QFile file(qssFile);
        if (file.open(QFile::ReadOnly)) {
            QString qss = QLatin1String(file.readAll());
          //  qDebug ()<<qss;
            qApp->setStyleSheet(qss);
          //  QString PaletteColor = qss.mid(20, 7);
          //  qApp->setPalette(QPalette(QColor(PaletteColor)));
            file.close();
        }
    }


    //判断是否是IP地址
    static bool IsIP(QString IP) {
        QRegExp RegExp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
        return RegExp.exactMatch(IP);
    }
    //验证是否是汉字-数字-字母-下划线
    static bool IsRegularName(QString name)
    {

        QRegExp RegExp("([_a-zA-Z0-9\u4e00-\u9fa5])+");
        return RegExp.exactMatch(name);
    }

    //显示输入框
    static QString ShowInputBox(QString info, bool &ok) {
        MyInputBox input;
        input.SetMessage(info);
        ok = input.exec();
        return input.GetValue();
    }

    //显示信息框,仅确定按钮
    static int ShowMessageBoxInfo(const QString msg,const QString title="提示") {
        MyMessageBox message ;
       message.SetMessage(msg,title,0);
        return message.exec();
    }


    //显示询问框,确定和取消按钮
    static int ShowMessageBoxQuesion(const QString msg,const QString title="提示") {
        MyMessageBox message;
        message.SetMessage(msg,title,1);
        return message.exec();
    }

    //16进制字符串转字节数组
    static QByteArray HexStrToByteArray(QString str) {
        QByteArray senddata;
        int hexdata, lowhexdata;
        int hexdatalen = 0;
        int len = str.length();
        senddata.resize(len / 2);
        char lstr, hstr;
        for (int i = 0; i < len; ) {
            hstr = str[i].toLatin1();
            if (hstr == ' ') {
                i++;
                continue;
            }
            i++;
            if (i >= len) {
                break;
            }
            lstr = str[i].toLatin1();
            hexdata = ConvertHexChar(hstr);
            lowhexdata = ConvertHexChar(lstr);
            if ((hexdata == 16) || (lowhexdata == 16)) {
                break;
            } else {
                hexdata = hexdata * 16 + lowhexdata;
            }
            i++;
            senddata[hexdatalen] = (char)hexdata;
            hexdatalen++;
        }
        senddata.resize(hexdatalen);
        return senddata;
    }

    static char ConvertHexChar(char ch) {
        if ((ch >= '0') && (ch <= '9')) {
            return ch - 0x30;
        } else if ((ch >= 'A') && (ch <= 'F')) {
            return ch - 'A' + 10;
        } else if ((ch >= 'a') && (ch <= 'f')) {
            return ch - 'a' + 10;
        } else {
            return (-1);
        }
    }

    //字节数组转16进制字符串
    static QString ByteArrayToHexStr(QByteArray data) {
        QString temp = "";
        QString hex = data.toHex();
        for (int i = 0; i < hex.length(); i = i + 2) {
            temp += hex.mid(i, 2) + " ";
        }
        return temp.trimmed().toUpper();
    }

    //16进制字符串转10进制
    static int StrHexToDecimal(QString strHex) {
        bool ok;
        return strHex.toInt(&ok, 16);
    }

    //10进制字符串转10进制
    static int StrDecimalToDecimal(QString strDecimal) {
        bool ok;
        return strDecimal.toInt(&ok, 10);
    }

    //2进制字符串转10进制
    static int StrBinToDecimal(QString strBin) {
        bool ok;
        return strBin.toInt(&ok, 2);
    }

    //16进制字符串转2进制字符串
    static QString StrHexToStrBin(QString strHex) {
        uchar decimal = StrHexToDecimal(strHex);
        QString bin = QString::number(decimal, 2);
        uchar len = bin.length();
        if (len < 8) {
            for (int i = 0; i < 8 - len; i++) {
                bin = "0" + bin;
            }
        }
        return bin;
    }

    //10进制转2进制字符串一个字节
    static QString DecimalToStrBin1(int decimal) {
        QString bin = QString::number(decimal, 2);
        uchar len = bin.length();
        if (len <= 8) {
            for (int i = 0; i < 8 - len; i++) {
                bin = "0" + bin;
            }
        }
        return bin;
    }

    //10进制转2进制字符串两个字节
    static QString DecimalToStrBin2(int decimal) {
        QString bin = QString::number(decimal, 2);
        uchar len = bin.length();
        if (len <= 16) {
            for (int i = 0; i < 16 - len; i++) {
                bin = "0" + bin;
            }
        }
        return bin;
    }

    //10进制转16进制字符串,补零.
    static QString DecimalToStrHex(int decimal) {
        QString temp = QString::number(decimal, 16);
        if (temp.length() == 1) {
            temp = "0" + temp;
        }
        return temp;
    }

    //延时
    static void Sleep(int sec) {
        QTime dieTime = QTime::currentTime().addMSecs(sec);
        while ( QTime::currentTime() < dieTime ) {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
    }
   //获取当前系统时间戳
    static qint64 GetSystemTimeStr()
    {
        QDateTime dt;
        QTime time;
        QDate date;
        dt.setTime(time.currentTime());
        dt.setDate(date.currentDate());
        return dt.currentMSecsSinceEpoch();
    }

    //设置系统日期时间
    static void SetSystemDateTime(int year, int month, int day, int hour, int min, int sec) {
        QProcess p(0);

        p.start("cmd");
        p.waitForStarted();
        p.write(QString("date %1-%2-%3\n").arg(year).arg(month).arg(day).toLatin1());
        p.closeWriteChannel();
        p.waitForFinished(1000);
        p.close();

        p.start("cmd");
        p.waitForStarted();
        p.write(QString("time %1:%2:%3.00\n").arg(hour).arg(min).arg(sec).toLatin1());
        p.closeWriteChannel();
        p.waitForFinished(1000);
        p.close();
    }

    //窗体居中显示
    static void FormInCenter(QWidget *frm) {
        QDesktopWidget* desktopWidget = QApplication::desktop();
        //得到客户区矩形
        QRect clientRect = desktopWidget->availableGeometry();
        int deskWidth = clientRect.width();
        int deskHeigth = clientRect.height();
        int frmX = frm->width();
        int frmY = frm->height();
        QPoint movePoint(deskWidth / 2 - frmX / 2, deskHeigth / 2 - frmY / 2);
        frm->move(movePoint);
    }

    //获取客户端屏幕大小
    static QRect getDesktopRect(){
        QDesktopWidget* desktopWidget = QApplication::desktop();
        return desktopWidget->availableGeometry();
    }

    //获取选择的文件
    static QString GetFileName(QString filter) {
        return QFileDialog::getOpenFileName(0, "选择文件", QCoreApplication::applicationDirPath(), filter);
    }

    //获取选择的文件集合
    static QStringList GetFileNames(QString filter) {
        return QFileDialog::getOpenFileNames(0, "选择文件", QCoreApplication::applicationDirPath(), filter);
    }

    //获取选择的目录
    static QString GetFolderName() {
        return QFileDialog::getExistingDirectory();;
    }

    //获取文件名,含拓展名
    static QString GetFileNameWithExtension(QString strFilePath) {
        QFileInfo fileInfo(strFilePath);
        return fileInfo.fileName();
    }

    //获取选择文件夹中的文件
    static QStringList GetFolderFileNames(QStringList filter) {
        QStringList fileList;
        QString strFolder = QFileDialog::getExistingDirectory();
        if (strFolder.length() != 0) {
            QDir myFolder(strFolder);
            if (myFolder.exists()) {
                fileList = myFolder.entryList(filter);
            }
        }
        return fileList;
    }

    //文件夹是否存在
    static bool FolderIsExist(QString strFolder) {
        QDir tempFolder(strFolder);
        return tempFolder.exists();
    }

    //文件是否存在
    static bool FileIsExist(QString strFile) {
        QFile tempFile(strFile);
        return tempFile.exists();
    }

    //复制文件
    static bool CopyFile(QString sourceFile, QString targetFile) {
        if (FileIsExist(targetFile)) {
            if (ShowMessageBoxQuesion("文件已经存在,是否替换?") == 1) {
                return false;
            }
        }
        return QFile::copy(sourceFile, targetFile);
    }

    //异或加密算法
    static QString getXorEncryptDecrypt(QString str, char key) {
        QByteArray data = str.toLatin1();
        int size = data.size();
        for (int i = 0; i < size; i++) {
            data[i] = data[i] ^ key;
        }
        return QLatin1String(data);
    }

    //md5加密
    static QString getMD5(QString str){
        QByteArray bytePwd = str.toLatin1();
        QByteArray bytePwdMd5 = QCryptographicHash::hash(bytePwd, QCryptographicHash::Md5);
        return bytePwdMd5.toHex();
    }
    //获取程序当前运行路径
    static QString getApplicationPath(){
        QDir dir;
       return  dir.currentPath();
    }
};
#endif // MYHELPER
