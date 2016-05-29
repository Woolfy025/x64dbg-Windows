#ifndef CPUARGUMENTWIDGET_H
#define CPUARGUMENTWIDGET_H

#include <QWidget>
#include <vector>
#include "StdTable.h"
#include "StringUtil.h"

namespace Ui
{
    class CPUArgumentWidget;
}

class CPUArgumentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CPUArgumentWidget(QWidget* parent = 0);
    ~CPUArgumentWidget();

    static QString defaultArgFormat(const QString & format, const QString & expression)
    {
        if(format.length())
            return format;
        return QString("%1 {a:%1}").arg(expression);
    }

    static QString defaultArgName(const QString & name, int argN)
    {
        if(name.length())
            return name;
        return QString("%1").arg(argN);
    }

    static QString defaultArgFieldFormat(const QString & argName, const QString & argText)
    {
        if(argText.length())
            return QString("%1: %2").arg(argName).arg(argText);
        return "";
    }

public slots:
    void disassembledAtSlot(dsint addr, dsint cip, bool history, dsint newTableOffset);
    void refreshData();

private slots:
    void on_comboCallingConvention_currentIndexChanged(int index);
    void on_spinArgCount_valueChanged(int arg1);
    void on_checkBoxLock_stateChanged(int arg1);

private:
    struct Argument
    {
        QString name;
        QString expression32;
        QString expression64;
        QString format32;
        QString format64;

        const QString & getExpression() const
        {
            return ArchValue(expression32, expression64);
        }

        QString getFormat() const
        {
            return CPUArgumentWidget::defaultArgFormat(ArchValue(format32, format64), getExpression());
        }

        explicit Argument(const QString & name, const QString & expression32, const QString & expression64, const QString & format32 = "", const QString & format64 = "")
            : name(name),
              expression32(expression32),
              expression64(expression64),
              format32(format32),
              format64(format64)
        {
        }
    };

    struct CallingConvention
    {
        QString name;
        int stackArgCount;
        QString stackLocation32;
        duint callOffset32;
        QString stackLocation64;
        duint callOffset64;
        std::vector<Argument> arguments;

        const QString & getStackLocation() const
        {
            return ArchValue(stackLocation32, stackLocation64);
        }

        const duint & getCallOffset() const
        {
            return ArchValue(callOffset32, callOffset64);
        }

        void addArgument(const Argument & argument)
        {
            arguments.push_back(argument);
        }

        explicit CallingConvention(const QString & name, int stackArgCount = 0, const QString & stackLocation32 = "esp", duint callOffset32 = sizeof(duint), const QString & stackLocation64 = "rsp", duint callOffset64 = sizeof(duint))
            : name(name),
              stackArgCount(stackArgCount),
              stackLocation32(stackLocation32),
              callOffset32(callOffset32),
              stackLocation64(stackLocation64),
              callOffset64(callOffset64)
        {
        }
    };

    Ui::CPUArgumentWidget* ui;
    StdTable* mTable;
    int mCurrentCallingConvention;
    duint mStackOffset;
    bool mAllowUpdate;
    std::vector<CallingConvention> mCallingConventions;

    void loadConfig();
    void setupTable();
};

#endif // CPUARGUMENTWIDGET_H