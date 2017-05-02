#include "RigidBodyFromObj.h"

#include "btBulletDynamicsCommon.h"
#include "LinearMath/btVector3.h"
#include "LinearMath/btAlignedObjectArray.h"
#include "../CommonInterfaces/CommonRigidBodyBase.h"

#include "../Utils/b3ResourcePath.h"
#include "Bullet3Common/b3FileUtils.h"
#include "../Importers/ImportObjDemo/LoadMeshFromObj.h"
#include "../OpenGLWindow/GLInstanceGraphicsShape.h"


btRigidBody* LoadMeshFromObj(std::string collisionFile, float* pos, float* scale, float* rot){
	const char* fileName = collisionFile;
	char relativeFileName[1024];
	if (b3ResourcePath::findResourcePath(fileName, relativeFileName, 1024)) {
		char pathPrefix[1024];
		b3FileUtils::extractPath(relativeFileName, pathPrefix, 1024);
	}
	GLInstanceGraphicsShape* glmesh = LoadMeshFromObj(relativeFileName, "");
	printf("[INFO] Obj loaded: Extracted %d verticed from obj file [%s]\n", glmesh->m_numvertices, fileName);
	const GLInstanceVertex& v = glmesh->m_vertices->at(0);
	btConvexHullShape* shape = new btConvexHullShape((const btScalar*)(&(v.xyzw[0])), glmesh->m_numvertices, sizeof(GLInstanceVertex));
	btVector3 color(1,1,1);
	btVector3 scaling(scale[0], scale[1], scale[2]);
	shape->setLocalScaling(scaling);
	if (m_options & OptimizeConvexObj)
	{
		shape->optimizeConvexHull();
	}
	if (m_options & ComputePolyhedralFeatures)
	{
		shape->initializePolyhedralFeatures();
	}
	m_collisionShapes.push_back(shape);
	btTransform startTransform;
	startTransform.setIdentity();
	btScalar mass(1.f);
	btVector3 position(pos[0], pos[1], pos[2]);
	startTransform.setOrigin(position);
	btRigidBody* body = createRigidBody(mass,startTransform,shape);
	btRigidBody * rigidBody = //...
	btTransform tr;
	tr.setIdentity();
	btQuaternion quat;
	quat.setEuler(rot[0],rot[1],rot[2]); //or quat.setEulerZYX depending on the ordering you want
	tr.setRotation(quat);
	rigidBody->setCenterOfMassTransform(tr);
	dynamicsWorld->addRigidBody(rigidBody);
}
