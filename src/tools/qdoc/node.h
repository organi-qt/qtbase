/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the tools applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

/*
  node.h
*/

#ifndef NODE_H
#define NODE_H

#include <qdir.h>
#include <qmap.h>
#include <qpair.h>
#include <qstringlist.h>

#include "codechunk.h"
#include "doc.h"
#include "location.h"
#include "text.h"

QT_BEGIN_NAMESPACE

class Node;
class ClassNode;
class InnerNode;
class ClassNode;
class ExampleNode;
class QmlClassNode;
class Tree;

typedef QMap<QString, const Node*> NodeMap;
typedef QMultiMap<QString, Node*> NodeMultiMap;
typedef QMultiMap<QString, const ExampleNode*> ExampleNodeMap;
typedef QList<QPair<QString,QString> > ImportList;

class Node
{
public:
    enum Type {
        Namespace,
        Class,
        Fake,
        Enum,
        Typedef,
        Function,
        Property,
        Variable,
        QmlProperty,
        QmlSignal,
        QmlSignalHandler,
        QmlMethod,
        LastType
    };

    enum SubType {
        NoSubType,
        Example,
        HeaderFile,
        File,
        Image,
        Group,
        Module,
        Page,
        ExternalPage,
        QmlClass,
        QmlPropertyGroup,
        QmlBasicType,
        QmlModule,
        DitaMap,
        Collision,
        LastSubtype
    };

    enum Access { Public, Protected, Private };

    enum Status {
        Compat,
        Obsolete,
        Deprecated,
        Preliminary,
        Commendable,
        Main,
        Internal
    }; // don't reorder this enum

    enum ThreadSafeness {
        UnspecifiedSafeness,
        NonReentrant,
        Reentrant,
        ThreadSafe
    };

    enum LinkType {
        StartLink,
        NextLink,
        PreviousLink,
        ContentsLink,
        IndexLink,
        InheritsLink /*,
        GlossaryLink,
        CopyrightLink,
        ChapterLink,
        SectionLink,
        SubsectionLink,
        AppendixLink */
    };

    enum PageType {
        NoPageType,
        ApiPage,
        ArticlePage,
        ExamplePage,
        HowToPage,
        OverviewPage,
        TutorialPage,
        FAQPage,
        DitaMapPage,
        OnBeyondZebra
    };

    enum FlagValue {
        FlagValueDefault = -1,
        FlagValueFalse = 0,
        FlagValueTrue = 1
    };

    virtual ~Node();

    void setAccess(Access access) { access_ = access; }
    void setLocation(const Location& location) { loc = location; }
    void setDoc(const Doc& doc, bool replace = false);
    void setStatus(Status status) { status_ = status; }
    void setThreadSafeness(ThreadSafeness safeness) { safeness_ = safeness; }
    void setSince(const QString &since);
    void setRelates(InnerNode* pseudoParent);
    void setModuleName(const QString &module) { mod = module; }
    void setLink(LinkType linkType, const QString &link, const QString &desc);
    void setUrl(const QString &url);
    void setTemplateStuff(const QString &templateStuff) { templateStuff_ = templateStuff; }
    void setPageType(PageType t) { pageType_ = t; }
    void setPageType(const QString& t);
    void setParent(InnerNode* n) { parent_ = n; }
    void setIndexNodeFlag() { indexNodeFlag_ = true; }
    virtual void setOutputFileName(const QString& ) { }

    virtual bool isInnerNode() const = 0;
    virtual bool isLeaf() const { return false; }
    virtual bool isReimp() const { return false; }
    virtual bool isFunction() const { return false; }
    virtual bool isQmlNode() const { return false; }
    virtual bool isQtQuickNode() const { return false; }
    virtual bool isAbstract() const { return false; }
    virtual bool isQmlPropertyGroup() const { return false; }
    virtual bool isCollisionNode() const { return false; }
    virtual bool isAttached() const { return false; }
    virtual void setAbstract(bool ) { }
    virtual QString title() const { return QString(); }
    bool isInternal() const;
    bool isIndexNode() const { return indexNodeFlag_; }
    Type type() const { return nodeType_; }
    virtual SubType subType() const { return NoSubType; }
    InnerNode* parent() const { return parent_; }
    InnerNode* relates() const { return relatesTo_; }
    const QString& name() const { return name_; }
    QMap<LinkType, QPair<QString,QString> > links() const { return linkMap; }
    QString moduleName() const;
    QString url() const;
    virtual QString nameForLists() const { return name_; }
    virtual QString outputFileName() const { return QString(); }
    virtual void addGroupMember(Node* ) { }
    virtual void addQmlModuleMember(Node* ) { }

    Access access() const { return access_; }
    QString accessString() const;
    const Location& location() const { return loc; }
    const Doc& doc() const { return d; }
    Status status() const { return status_; }
    Status inheritedStatus() const;
    ThreadSafeness threadSafeness() const;
    ThreadSafeness inheritedThreadSafeness() const;
    QString since() const { return sinc; }
    QString templateStuff() const { return templateStuff_; }
    PageType pageType() const { return pageType_; }
    QString pageTypeString() const;
    QString nodeTypeString() const;
    QString nodeSubtypeString() const;
    virtual void addPageKeywords(const QString& ) { }

    void clearRelated() { relatesTo_ = 0; }

    virtual QString fileBase() const;
    QString guid() const;
    QString ditaXmlHref();
    QString extractClassName(const QString &string) const;
    virtual QString qmlTypeName() const { return name_; }
    virtual QString qmlModuleName() const { return qmlModuleName_; }
    virtual QString qmlModuleVersion() const { return qmlModuleVersionMajor_ + "." + qmlModuleVersionMinor_; }
    virtual QString qmlModuleIdentifier() const { return qmlModuleName_ + qmlModuleVersionMajor_; }
    virtual bool setQmlModule(const ArgLocPair& );
    virtual ClassNode* classNode() { return 0; }
    virtual void clearCurrentChild() { }
    virtual const ImportList* importList() const { return 0; }
    virtual void setImportList(const ImportList& ) { }
    virtual const Node* applyModuleIdentifier(const Node* ) const { return 0; }
    virtual QString idNumber() { return "0"; }
    QmlClassNode* qmlClassNode();
    ClassNode* declarativeCppNode();
    const QString& outputSubdirectory() const { return outSubDir_; }
    QString fullDocumentName() const;
    static QString cleanId(QString str);
    QString idForNode() const;

    static FlagValue toFlagValue(bool b);
    static bool fromFlagValue(FlagValue fv, bool defaultValue);

    static QString pageTypeString(unsigned t);
    static QString nodeTypeString(unsigned t);
    static QString nodeSubtypeString(unsigned t);
    static int incPropertyGroupCount();
    static void clearPropertyGroupCount();

protected:
    Node(Type type, InnerNode* parent, const QString& name);

private:

    Type nodeType_;
    Access access_;
    ThreadSafeness safeness_;
    PageType pageType_;
    Status status_;
    bool indexNodeFlag_;

#if 0
    //ifdef Q_WS_WIN
    //else
    Type nodeType_ : 4;
    Access access_ : 2;
    ThreadSafeness safeness_ : 2;
    PageType pageType_ : 4;
    Status status_ : 3;
#endif
    InnerNode* parent_;
    InnerNode* relatesTo_;
    QString name_;
    Location loc;
    Doc d;
    QMap<LinkType, QPair<QString, QString> > linkMap;
    QString mod;
    QString url_;
    QString sinc;
    QString templateStuff_;
    mutable QString uuid_;
    QString outSubDir_;
    QString qmlModuleName_;
    QString qmlModuleVersionMajor_;
    QString qmlModuleVersionMinor_;
    static QStringMap operators_;
    static int propertyGroupCount_;
};

class FunctionNode;
class EnumNode;
class NameCollisionNode;

typedef QList<Node*> NodeList;
typedef QMap<QString, const Node*> NodeMap;
typedef QMultiMap<QString, Node*> NodeMultiMap;

class InnerNode : public Node
{
public:
    virtual ~InnerNode();

    Node* findChildNodeByName(const QString& name);
    Node* findChildNodeByName(const QString& name, bool qml);
    Node* findChildNodeByNameAndType(const QString& name, Type type);
    void findNodes(const QString& name, QList<Node*>& n);
    FunctionNode* findFunctionNode(const QString& name);
    FunctionNode* findFunctionNode(const FunctionNode* clone);
    void addInclude(const QString &include);
    void setIncludes(const QStringList &includes);
    void setOverload(const FunctionNode* func, bool overlode);
    void normalizeOverloads();
    void makeUndocumentedChildrenInternal();
    void clearCurrentChildPointers();
    void deleteChildren();
    void removeFromRelated();

    virtual bool isInnerNode() const { return true; }
    virtual bool isLeaf() const { return false; }
    const Node* findChildNodeByName(const QString& name) const;
    const Node* findChildNodeByName(const QString& name, bool qml) const;
    const Node* findChildNodeByNameAndType(const QString& name, Type type) const;
    const FunctionNode* findFunctionNode(const QString& name) const;
    const FunctionNode* findFunctionNode(const FunctionNode* clone) const;
    const EnumNode* findEnumNodeForValue(const QString &enumValue) const;
    const NodeList & childNodes() const { return children; }
    const NodeList & relatedNodes() const { return related_; }
    int count() const { return children.size(); }
    int overloadNumber(const FunctionNode* func) const;
    NodeList overloads(const QString &funcName) const;
    const QStringList& includes() const { return inc; }

    QStringList primaryKeys();
    QStringList secondaryKeys();
    const QStringList& pageKeywords() const { return pageKeywds; }
    virtual void addPageKeywords(const QString& t) { pageKeywds << t; }
    virtual void setCurrentChild() { }
    virtual void setCurrentChild(InnerNode* ) { }
    virtual void setOutputFileName(const QString& f) { outputFileName_ = f; }
    virtual QString outputFileName() const { return outputFileName_; }

protected:
    InnerNode(Type type, InnerNode* parent, const QString& name);

private:
    friend class Node;
    friend class NameCollisionNode;

    static bool isSameSignature(const FunctionNode* f1, const FunctionNode* f2);
    void addChild(Node* child);
    void removeRelated(Node* pseudoChild);
    void removeChild(Node* child);

    QString outputFileName_;
    QStringList pageKeywds;
    QStringList inc;
    NodeList children;
    NodeList enumChildren;
    NodeList related_;
    QMap<QString, Node*> childMap;
    QMap<QString, Node*> primaryFunctionMap;
    QMap<QString, NodeList> secondaryFunctionMap;
};

class LeafNode : public Node
{
public:
    LeafNode();
    virtual ~LeafNode() { }

    virtual bool isInnerNode() const { return false; }
    virtual bool isLeaf() const { return true; }

protected:
    LeafNode(Type type, InnerNode* parent, const QString& name);
    LeafNode(InnerNode* parent, Type type, const QString& name);
};

class NamespaceNode : public InnerNode
{
public:
    NamespaceNode(InnerNode* parent, const QString& name);
    virtual ~NamespaceNode() { }
};

class ClassNode;

struct RelatedClass
{
    RelatedClass() { }
    RelatedClass(Node::Access access0,
                 ClassNode* node0,
                 const QString& dataTypeWithTemplateArgs0 = "")
        : access(access0),
          node(node0),
          dataTypeWithTemplateArgs(dataTypeWithTemplateArgs0) { }
    QString accessString() const;

    Node::Access        access;
    ClassNode*          node;
    QString             dataTypeWithTemplateArgs;
};

class PropertyNode;

class ClassNode : public InnerNode
{
public:
    ClassNode(InnerNode* parent, const QString& name);
    virtual ~ClassNode() { }

    void addBaseClass(Access access,
                      ClassNode* node,
                      const QString &dataTypeWithTemplateArgs = "");
    void fixBaseClasses();

    const QList<RelatedClass> &baseClasses() const { return bases; }
    const QList<RelatedClass> &derivedClasses() const { return derived; }
    const QList<RelatedClass> &ignoredBaseClasses() const { return ignoredBases; }

    bool hideFromMainList() const { return hidden; }
    void setHideFromMainList(bool value) { hidden = value; }

    QString serviceName() const { return sname; }
    void setServiceName(const QString& value) { sname = value; }
    QmlClassNode* qmlElement() { return qmlelement; }
    void setQmlElement(QmlClassNode* qcn) { qmlelement = qcn; }
    virtual bool isAbstract() const { return abstract; }
    virtual void setAbstract(bool b) { abstract = b; }
    PropertyNode* findPropertyNode(const QString& name);
    QmlClassNode* findQmlBaseNode();

private:
    QList<RelatedClass> bases;
    QList<RelatedClass> derived;
    QList<RelatedClass> ignoredBases;
    bool hidden;
    bool abstract;
    QString sname;
    QmlClassNode* qmlelement;
};

class FakeNode : public InnerNode
{
public:

    FakeNode(InnerNode* parent,
             const QString& name,
             SubType subType,
             PageType ptype);
    virtual ~FakeNode() { }

    void setTitle(const QString &title) { title_ = title; }
    void setSubTitle(const QString &subTitle) { subtitle_ = subTitle; }
    virtual void addGroupMember(Node* node) { nodeList.append(node); }
    virtual void addQmlModuleMember(Node* node) { nodeList.append(node); }

    SubType subType() const { return nodeSubtype_; }
    virtual QString title() const;
    virtual QString fullTitle() const;
    virtual QString subTitle() const;
    virtual QString imageFileName() const { return QString(); }
    const NodeList& groupMembers() const { return nodeList; }
    const NodeList& qmlModuleMembers() const { return nodeList; }
    virtual QString nameForLists() const { return title(); }
    virtual void setImageFileName(const QString& ) { }
    virtual bool isQmlPropertyGroup() const { return (nodeSubtype_ == QmlPropertyGroup); }

    static void insertQmlModuleNode(const QString& qmid, FakeNode* fn);
    static FakeNode* lookupQmlModuleNode(Tree* tree, const ArgLocPair& arg);

protected:
    SubType nodeSubtype_;
    QString title_;
    QString subtitle_;
    NodeList nodeList; // used for groups and QML modules.

    static QMap<QString, FakeNode*> qmlModuleMap_;
};

class NameCollisionNode : public FakeNode
{
public:
    NameCollisionNode(InnerNode* child);
    ~NameCollisionNode();
    const InnerNode* currentChild() const { return current; }
    virtual void setCurrentChild(InnerNode* child) { current = child; }
    virtual void clearCurrentChild() { current = 0; }
    virtual bool isQmlNode() const;
    virtual bool isCollisionNode() const { return true; }
    virtual const Node* applyModuleIdentifier(const Node* origin) const;
    InnerNode* findAny(Node::Type t, Node::SubType st);
    void addCollision(InnerNode* child);
    const QMap<QString,QString>& linkTargets() const { return targets; }
    void addLinkTarget(const QString& t, const QString& v) { targets.insert(t,v); }

private:
    InnerNode* current;
    QMap<QString,QString> targets;
};

class ExampleNode : public FakeNode
{
public:
    ExampleNode(InnerNode* parent, const QString& name);
    virtual ~ExampleNode() { }
    virtual QString imageFileName() const { return imageFileName_; }
    virtual void setImageFileName(const QString& ifn) { imageFileName_ = ifn; }

    static void terminate() { exampleNodeMap.clear(); }

public:
    static ExampleNodeMap exampleNodeMap;

private:
    QString imageFileName_;
};

class QmlClassNode : public FakeNode
{
public:
    QmlClassNode(InnerNode* parent,
                 const QString& name,
                 ClassNode* cn);
    virtual ~QmlClassNode();
    virtual bool isQmlNode() const { return true; }
    virtual bool isQtQuickNode() const { return (qmlModuleName() == QLatin1String("QtQuick")); }
    virtual ClassNode* classNode() { return cnode_; }
    virtual QString fileBase() const;
    virtual void setCurrentChild();
    virtual void clearCurrentChild();
    virtual const ImportList* importList() const { return &importList_; }
    virtual void setImportList(const ImportList& il) { importList_ = il; }
    virtual bool isAbstract() const { return abstract; }
    virtual void setAbstract(bool b) { abstract = b; }
    const FakeNode* qmlBase() const { return base_; }
    void resolveInheritance(Tree* tree);
    static void addInheritedBy(const QString& base, Node* sub);
    static void subclasses(const QString& base, NodeList& subs);
    static void terminate();

    static void insertQmlModuleMember(const QString& qmid, QmlClassNode* qcn);
    static QmlClassNode* lookupQmlTypeNode(const QString& qmid, const QString& name);

public:
    static bool qmlOnly;
    static QMultiMap<QString,Node*> inheritedBy;
    static QMap<QString, QmlClassNode*> qmlModuleMemberMap_;

private:
    bool abstract;
    ClassNode*    cnode_;
    FakeNode*     base_;
    ImportList          importList_;
};

class QmlBasicTypeNode : public FakeNode
{
public:
    QmlBasicTypeNode(InnerNode* parent,
                     const QString& name);
    virtual ~QmlBasicTypeNode() { }
    virtual bool isQmlNode() const { return true; }
};

class QmlPropGroupNode : public FakeNode
{
public:
    QmlPropGroupNode(QmlClassNode* parent, const QString& name);
    virtual ~QmlPropGroupNode() { }
    virtual bool isQmlNode() const { return true; }
    virtual bool isQtQuickNode() const { return parent()->isQtQuickNode(); }
    virtual QString qmlTypeName() const { return parent()->qmlTypeName(); }
    virtual QString qmlModuleName() const { return parent()->qmlModuleName(); }
    virtual QString qmlModuleVersion() const { return parent()->qmlModuleVersion(); }
    virtual QString qmlModuleIdentifier() const { return parent()->qmlModuleIdentifier(); }
    virtual QString idNumber();

    const QString& element() const { return parent()->name(); }

 private:
    int     idNumber_;
};

class QmlPropertyNode;

class QmlPropertyNode : public LeafNode
{
public:
    QmlPropertyNode(QmlClassNode *parent,
                    const QString& name,
                    const QString& type,
                    bool attached);
    QmlPropertyNode(QmlPropGroupNode* parent,
                    const QString& name,
                    const QString& type,
                    bool attached);
    QmlPropertyNode(QmlPropertyNode* parent,
                    const QString& name,
                    const QString& type,
                    bool attached);
    virtual ~QmlPropertyNode() { }

    void setDataType(const QString& dataType) { type_ = dataType; }
    void setStored(bool stored) { stored_ = toFlagValue(stored); }
    void setDesignable(bool designable) { designable_ = toFlagValue(designable); }
    void setReadOnly(bool ro) { readOnly_ = toFlagValue(ro); }
    void setDefault() { isdefault_ = true; }

    const QString &dataType() const { return type_; }
    QString qualifiedDataType() const { return type_; }
    bool isReadOnlySet() const { return (readOnly_ != FlagValueDefault); }
    bool isDefault() const { return isdefault_; }
    bool isStored() const { return fromFlagValue(stored_,true); }
    bool isDesignable() const { return fromFlagValue(designable_,false); }
    bool isWritable(Tree* tree);
    bool isReadOnly() const { return fromFlagValue(readOnly_,false); }
    virtual bool isAttached() const { return attached_; }
    virtual bool isQmlNode() const { return true; }
    virtual bool isQtQuickNode() const { return parent()->isQtQuickNode(); }
    virtual QString qmlTypeName() const { return parent()->qmlTypeName(); }
    virtual QString qmlModuleName() const { return parent()->qmlModuleName(); }
    virtual QString qmlModuleVersion() const { return parent()->qmlModuleVersion(); }
    virtual QString qmlModuleIdentifier() const { return parent()->qmlModuleIdentifier(); }

    PropertyNode* correspondingProperty(Tree* tree);

    const QString& element() const { return static_cast<QmlPropGroupNode*>(parent())->element(); }
    void appendQmlPropNode(QmlPropertyNode* p) { qmlPropNodes_.append(p); }
    const NodeList& qmlPropNodes() const { return qmlPropNodes_; }
    void setQPropertyFlag() { qproperty_ = true; }

private:
    QString type_;
    FlagValue   stored_;
    FlagValue   designable_;
    bool    isdefault_;
    bool    attached_;
    bool        qproperty_;
    FlagValue   readOnly_;
    NodeList qmlPropNodes_;
};

class EnumItem
{
public:
    EnumItem() { }
    EnumItem(const QString& name, const QString& value)
        : nam(name), val(value) { }
    EnumItem(const QString& name, const QString& value, const Text &txt)
        : nam(name), val(value), txt(txt) { }

    const QString& name() const { return nam; }
    const QString& value() const { return val; }
    const Text &text() const { return txt; }

private:
    QString nam;
    QString val;
    Text txt;
};

class TypedefNode;

class EnumNode : public LeafNode
{
public:
    EnumNode(InnerNode* parent, const QString& name);
    virtual ~EnumNode() { }

    void addItem(const EnumItem& item);
    void setFlagsType(TypedefNode* typedeff);
    bool hasItem(const QString &name) const { return names.contains(name); }

    const QList<EnumItem>& items() const { return itms; }
    Access itemAccess(const QString& name) const;
    const TypedefNode* flagsType() const { return ft; }
    QString itemValue(const QString &name) const;

private:
    QList<EnumItem> itms;
    QSet<QString> names;
    const TypedefNode* ft;
};

class TypedefNode : public LeafNode
{
public:
    TypedefNode(InnerNode* parent, const QString& name);
    virtual ~TypedefNode() { }

    const EnumNode* associatedEnum() const { return ae; }

private:
    void setAssociatedEnum(const EnumNode* enume);

    friend class EnumNode;

    const EnumNode* ae;
};

inline void EnumNode::setFlagsType(TypedefNode* typedeff)
{
    ft = typedeff;
    typedeff->setAssociatedEnum(this);
}


class Parameter
{
public:
    Parameter() {}
    Parameter(const QString& leftType,
              const QString& rightType = "",
              const QString& name = "",
              const QString& defaultValue = "");
    Parameter(const Parameter& p);

    Parameter& operator=(const Parameter& p);

    void setName(const QString& name) { nam = name; }

    bool hasType() const { return lef.length() + rig.length() > 0; }
    const QString& leftType() const { return lef; }
    const QString& rightType() const { return rig; }
    const QString& name() const { return nam; }
    const QString& defaultValue() const { return def; }

    QString reconstruct(bool value = false) const;

private:
    QString lef;
    QString rig;
    QString nam;
    QString def;
};

class PropertyNode;

class FunctionNode : public LeafNode
{
public:
    enum Metaness {
        Plain,
        Signal,
        Slot,
        Ctor,
        Dtor,
        MacroWithParams,
        MacroWithoutParams,
        Native };
    enum Virtualness { NonVirtual, ImpureVirtual, PureVirtual };

    FunctionNode(InnerNode* parent, const QString &name);
    FunctionNode(Type type, InnerNode* parent, const QString &name, bool attached);
    virtual ~FunctionNode() { }

    void setReturnType(const QString& returnType) { rt = returnType; }
    void setParentPath(const QStringList& parentPath) { pp = parentPath; }
    void setMetaness(Metaness metaness) { met = metaness; }
    void setVirtualness(Virtualness virtualness);
    void setConst(bool conste) { con = conste; }
    void setStatic(bool statique) { sta = statique; }
    void setOverload(bool overlode);
    void setReimp(bool r);
    void addParameter(const Parameter& parameter);
    inline void setParameters(const QList<Parameter>& parameters);
    void borrowParameterNames(const FunctionNode* source);
    void setReimplementedFrom(FunctionNode* from);

    const QString& returnType() const { return rt; }
    Metaness metaness() const { return met; }
    bool isMacro() const {
        return met == MacroWithParams || met == MacroWithoutParams;
    }
    Virtualness virtualness() const { return vir; }
    bool isConst() const { return con; }
    bool isStatic() const { return sta; }
    bool isOverload() const { return ove; }
    bool isReimp() const { return reimp; }
    bool isFunction() const { return true; }
    int overloadNumber() const;
    const QList<Parameter>& parameters() const { return params; }
    QStringList parameterNames() const;
    QString rawParameters(bool names = false, bool values = false) const;
    const FunctionNode* reimplementedFrom() const { return rf; }
    const QList<FunctionNode*> &reimplementedBy() const { return rb; }
    const PropertyNode* associatedProperty() const { return ap; }
    const QStringList& parentPath() const { return pp; }

    QStringList reconstructParams(bool values = false) const;
    QString signature(bool values = false) const;
    const QString& element() const { return parent()->name(); }
    virtual bool isAttached() const { return attached_; }
    virtual bool isQmlNode() const {
        return ((type() == QmlSignal) ||
                (type() == QmlMethod) ||
                (type() == QmlSignalHandler));
    }
    virtual bool isQtQuickNode() const { return parent()->isQtQuickNode(); }
    virtual QString qmlTypeName() const { return parent()->qmlTypeName(); }
    virtual QString qmlModuleName() const { return parent()->qmlModuleName(); }
    virtual QString qmlModuleVersion() const { return parent()->qmlModuleVersion(); }
    virtual QString qmlModuleIdentifier() const { return parent()->qmlModuleIdentifier(); }

    void debug() const;

private:
    void setAssociatedProperty(PropertyNode* property);

    friend class InnerNode;
    friend class PropertyNode;

    QString     rt;
    QStringList pp;
#ifdef Q_WS_WIN
    Metaness    met;
    Virtualness vir;
#else
    Metaness met : 4;
    Virtualness vir : 2;
#endif
    bool con : 1;
    bool sta : 1;
    bool ove : 1;
    bool reimp: 1;
    bool attached_: 1;
    QList<Parameter> params;
    const FunctionNode* rf;
    const PropertyNode* ap;
    QList<FunctionNode*> rb;
};

class PropertyNode : public LeafNode
{
public:
    enum FunctionRole { Getter, Setter, Resetter, Notifier };
    enum { NumFunctionRoles = Notifier + 1 };

    PropertyNode(InnerNode* parent, const QString& name);
    virtual ~PropertyNode() { }

    void setDataType(const QString& dataType) { type_ = dataType; }
    void addFunction(FunctionNode* function, FunctionRole role);
    void addSignal(FunctionNode* function, FunctionRole role);
    void setStored(bool stored) { stored_ = toFlagValue(stored); }
    void setDesignable(bool designable) { designable_ = toFlagValue(designable); }
    void setScriptable(bool scriptable) { scriptable_ = toFlagValue(scriptable); }
    void setWritable(bool writable) { writable_ = toFlagValue(writable); }
    void setUser(bool user) { user_ = toFlagValue(user); }
    void setOverriddenFrom(const PropertyNode* baseProperty);
    void setRuntimeDesFunc(const QString& rdf) { runtimeDesFunc = rdf; }
    void setRuntimeScrFunc(const QString& scrf) { runtimeScrFunc = scrf; }
    void setConstant() { cst = true; }
    void setFinal() { fnl = true; }
    void setRevision(int revision) { rev = revision; }

    const QString &dataType() const { return type_; }
    QString qualifiedDataType() const;
    NodeList functions() const;
    NodeList functions(FunctionRole role) const { return funcs[(int)role]; }
    NodeList getters() const { return functions(Getter); }
    NodeList setters() const { return functions(Setter); }
    NodeList resetters() const { return functions(Resetter); }
    NodeList notifiers() const { return functions(Notifier); }
    bool isStored() const { return fromFlagValue(stored_, storedDefault()); }
    bool isDesignable() const { return fromFlagValue(designable_, designableDefault()); }
    bool isScriptable() const { return fromFlagValue(scriptable_, scriptableDefault()); }
    const QString& runtimeDesignabilityFunction() const { return runtimeDesFunc; }
    const QString& runtimeScriptabilityFunction() const { return runtimeScrFunc; }
    bool isWritable() const { return fromFlagValue(writable_, writableDefault()); }
    bool isUser() const { return fromFlagValue(user_, userDefault()); }
    bool isConstant() const { return cst; }
    bool isFinal() const { return fnl; }
    const PropertyNode* overriddenFrom() const { return overrides; }

    bool storedDefault() const { return true; }
    bool userDefault() const { return false; }
    bool designableDefault() const { return !setters().isEmpty(); }
    bool scriptableDefault() const { return true; }
    bool writableDefault() const { return !setters().isEmpty(); }

private:
    QString type_;
    QString runtimeDesFunc;
    QString runtimeScrFunc;
    NodeList funcs[NumFunctionRoles];
    FlagValue stored_;
    FlagValue designable_;
    FlagValue scriptable_;
    FlagValue writable_;
    FlagValue user_;
    bool cst;
    bool fnl;
    int rev;
    const PropertyNode* overrides;
};

inline void FunctionNode::setParameters(const QList<Parameter> &parameters)
{
    params = parameters;
}

inline void PropertyNode::addFunction(FunctionNode* function, FunctionRole role)
{
    funcs[(int)role].append(function);
    function->setAssociatedProperty(this);
}

inline void PropertyNode::addSignal(FunctionNode* function, FunctionRole role)
{
    funcs[(int)role].append(function);
    function->setAssociatedProperty(this);
}

inline NodeList PropertyNode::functions() const
{
    NodeList list;
    for (int i = 0; i < NumFunctionRoles; ++i)
        list += funcs[i];
    return list;
}

class VariableNode : public LeafNode
{
public:
    VariableNode(InnerNode* parent, const QString &name);
    virtual ~VariableNode() { }

    void setLeftType(const QString &leftType) { lt = leftType; }
    void setRightType(const QString &rightType) { rt = rightType; }
    void setStatic(bool statique) { sta = statique; }

    const QString &leftType() const { return lt; }
    const QString &rightType() const { return rt; }
    QString dataType() const { return lt + rt; }
    bool isStatic() const { return sta; }

private:
    QString lt;
    QString rt;
    bool sta;
};

inline VariableNode::VariableNode(InnerNode* parent, const QString &name)
    : LeafNode(Variable, parent, name), sta(false)
{
    // nothing.
}

class DitaMapNode : public FakeNode
{
public:
    DitaMapNode(InnerNode* parent, const QString& name)
        : FakeNode(parent, name, Node::Page, Node::DitaMapPage) { }
    virtual ~DitaMapNode() { }

    const DitaRefList& map() const { return doc().ditamap(); }
};

QT_END_NAMESPACE

#endif
