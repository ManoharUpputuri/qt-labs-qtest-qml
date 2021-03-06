/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtDeclarative/qdeclarativeextensionplugin.h>
#include <QtDeclarative/qdeclarative.h>
#include <QtScript/qscriptvalue.h>
#include <QtScript/qscriptcontext.h>
#include <QtScript/qscriptcontextinfo.h>
#include <QtScript/qscriptengine.h>
#include "quicktestresult_p.h"
#include "quicktestevent_p.h"

QT_BEGIN_NAMESPACE

QML_DECLARE_TYPE(QuickTestResult)
QML_DECLARE_TYPE(QuickTestEvent)

// Copied from qdeclarativedebughelper_p.h in Qt, to avoid a dependency
// on a private header from Qt.
class Q_DECLARATIVE_EXPORT QDeclarativeDebugHelper
{
public:
    static QScriptEngine *getScriptEngine(QDeclarativeEngine *engine);
    static void setAnimationSlowDownFactor(qreal factor);
    static void enableDebugging();
};

static QScriptContext *qtest_find_frame(QScriptContext *ctx)
{
    qint32 frame = 1;
    if (ctx->argumentCount() > 0)
        frame = ctx->argument(0).toInt32();
    ++frame;    // Exclude the native function; start at its caller.
    while (ctx) {
        if (frame-- <= 0)
            break;
        ctx = ctx->parentContext();
    }
    return ctx;
}

static QScriptValue qtest_caller_file
    (QScriptContext *ctx, QScriptEngine *engine)
{
    ctx = qtest_find_frame(ctx);
    if (ctx) {
        QScriptContextInfo info(ctx);
        return engine->newVariant(info.fileName());
    }
    return engine->newVariant(QLatin1String(""));
}

static QScriptValue qtest_caller_line
    (QScriptContext *ctx, QScriptEngine *engine)
{
    ctx = qtest_find_frame(ctx);
    if (ctx) {
        QScriptContextInfo info(ctx);
        return engine->newVariant(info.lineNumber());
    }
    return engine->newVariant(qint32(0));
}

class QTestQmlModule : public QDeclarativeExtensionPlugin
{
    Q_OBJECT
public:
    virtual void registerTypes(const char *uri)
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("QtQuickTest"));
        qmlRegisterType<QuickTestResult>(uri,1,0,"TestResult");
        qmlRegisterType<QuickTestEvent>(uri,1,0,"TestEvent");
    }
    void initializeEngine(QDeclarativeEngine *engine, const char *)
    {
        // Install some helper functions in the global "Qt" object
        // for walking the stack and finding a caller's location.
        // Normally we would use an exception's backtrace, but JSC
        // only provides the top-most frame in the backtrace.
        QScriptEngine *eng = QDeclarativeDebugHelper::getScriptEngine(engine);
        QScriptValue qtObject
            = eng->globalObject().property(QLatin1String("Qt"));
        qtObject.setProperty
            (QLatin1String("qtest_caller_file"),
             eng->newFunction(qtest_caller_file));
        qtObject.setProperty
            (QLatin1String("qtest_caller_line"),
             eng->newFunction(qtest_caller_line));
    }
};

QT_END_NAMESPACE

#include "main.moc"

Q_EXPORT_PLUGIN2(qmltestplugin, QT_PREPEND_NAMESPACE(QTestQmlModule));
