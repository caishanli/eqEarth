#pragma once

#define EQ_IGNORE_GLEW
#include <eq/eq.h>

#include <osgViewer/View>

namespace eqEarth
{
class View : public eq::View
{
public:
    View( eq::Layout* parent );

protected:
    virtual ~View( );

public:
    void setSceneID( const eq::uint128_t& id );
    eq::uint128_t getSceneID( ) const { return _sceneID; }

    void setViewMatrix( const eq::Matrix4d& viewMatrix );
    const eq::Matrix4d& getViewMatrix( ) const { return _viewMatrix; }

    void setNearFar( double near, double far );
    void getNearFar( double& near, double& far ) const;

    void setWorldPointer( const eq::Vector3d& origin,
        const eq::Vector3d& direction );
    void getWorldPointer( eq::Vector3d& origin, eq::Vector3d& direction ) const;

    void setView( osgViewer::View *view );
    osgViewer::View* getView( ) { return _view; }
    const osgViewer::View* getView( ) const { return _view; }

private:
    class Proxy : public eq::fabric::Serializable
    {
    public:
        Proxy( View* view ) : _view( view ) {}

    protected:
        /** The changed parts of the view. */
        enum DirtyBits
        {
            DIRTY_SCENE   = eq::fabric::Serializable::DIRTY_CUSTOM << 0,
            DIRTY_CAMERA  = eq::fabric::Serializable::DIRTY_CUSTOM << 1,
            DIRTY_NEARFAR = eq::fabric::Serializable::DIRTY_CUSTOM << 2,
            DIRTY_POINTER = eq::fabric::Serializable::DIRTY_CUSTOM << 3
        };

        virtual void serialize( co::DataOStream& os,
            const uint64_t dirtyBits );
        virtual void deserialize( co::DataIStream& is, 
            const uint64_t dirtyBits );
        virtual void notifyNewVersion() { sync( ); }

    private:
        View* const _view;
        friend class eqEarth::View;
    };

    Proxy _proxy;
    friend class Proxy;
    eq::uint128_t _sceneID;
    eq::Matrix4d _viewMatrix;
    double _near, _far;
    eq::Vector3d _origin;
    eq::Vector3d _direction;

    osg::ref_ptr< osgViewer::View > _view;
};
}