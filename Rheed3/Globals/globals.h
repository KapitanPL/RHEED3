#ifndef GLBOLS_H
#define GLBOLS_H
#include <QSize>
#include <QString>
#include <QPoint>
#include <QStringList>

//logger
#include <thread>
#include <mutex>
#include <queue>
#include <fstream>
#include <condition_variable>

#include "./../common_includes.h"

class C_logger
{
public:
    virtual ~C_logger();

    C_logger(C_logger const & other) = delete;
    C_logger(C_logger && other) = delete;
    void operator=(C_logger const & other) = delete;

    static C_logger &getInstance();
    static void work(C_logger * pThis);

    virtual bool init(std::wstring sFileName);
    virtual void logMsg(std::wstring sMsg);
    virtual std::wstring getFileName();
    virtual uint32_t getLineCount();
private:
    C_logger();
    void swapQueues();

    bool m_bRun = true;
    std::wstring m_sFileName;
    std::wfstream m_file;
    std::thread * m_pworkThread = nullptr;
    std::mutex m_mutexSwap;
    std::mutex m_mutexRun;
    std::condition_variable m_condVar;
    std::queue<std::wstring> m_storeQueue;
    std::queue<std::wstring> m_bufferQueue;
    uint32_t m_uiLines = 0;
};

class C_globals
{
public:
    static C_globals & getInstance();
    C_globals(C_globals const & other) = delete;
    C_globals(C_globals && other) = delete;
    void operator=(C_globals const & other) = delete;


    ~C_globals();
    void Load();
    void Save();

	void GetCameraPluginPath(QString& sPath);
	void GetCameraPluginPath(std::wstring& Path);

    void setupLogger();

    C_logger * m_pLogger = nullptr;

    QString m_sConfigPath = "./config";
    QString m_sLogPath = "./Logs";
	QString m_sPluginPath = "./Plugins";
	QString m_sCameraPluginPath = "./plugins_cameras";
	QString m_sGlobalPath = "";
    //logLevel 0 = default, log anything sent to logger.
    //logLevel 1 = debug level, special code points can be sprinkled by additional debug code
    //logLevel 2 = deep debug Level.
    int m_iLogLevel = 0;
    //can contain special strings to identify special debug events
    QStringList m_listDebugEntries;
    QSize m_mainWindowSize = QSize(800,600);
    QPoint m_mainWindowPos = QPoint(20,20);
private:
    C_globals();
};

class C_LibLoader
{
public:
	C_LibLoader(std::wstring sLibName, I_PMS_V01 * master, S_Version minVersion);
	~C_LibLoader();
	bool IsValid();
	I_PMS_V01* GetInterface();
private:
	bool m_bValid = false;
	void* m_lib = nullptr;
	I_PMS_V01* m_servant = nullptr;
	I_PMS_V01* m_master = nullptr;
};

#endif // GLBOLS_H
