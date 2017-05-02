#include "PhysicsManager.h"
#include "RenderManager.h"

#include "BulletSceneNodeMotionState.h"
#include "BulletDebugDrawer.h"

#include "RigidBody.h"
#include "CompoundShape.h"
#include "CompareRigidBodies.h"
#include "CompareCompoundShapes.h"

void PhysicsManager::stepPhysicsSimulation(float timeInc){
	btScalar timeStep(timeInc);
	updateRigidBodies();
	updateRigidBodyVelocity(timeInc);
	dynamicsWorld->stepSimulation(timeStep, 10, .01667);
	dynamicsWorld->debugDrawWorld();
}
void PhysicsManager::setGravity(float* g){
	dynamicsWorld->setGravity(btVector3(g[0],g[1],g[2]));
}
void PhysicsManager::getGravity(float* g){
	btVector3 gravity = dynamicsWorld->getGravity();
	g[0] = gravity.x();
	g[1] = gravity.y();
	g[2] = gravity.z();
}
void PhysicsManager::getLinearVelocity(std::string rigidBodyName, float* v){
	RigidBody* rb = rigidBodies->tableRetrieve(&rigidBodyName);
	if(rb){
		btRigidBody* btRb = rb->getRigidBody();
		btVector3 linearVelocity = btRb->getLinearVelocity();
		v[0] = linearVelocity.x();
		v[1] = linearVelocity.y();
		v[2] = linearVelocity.z();
	}
}
void PhysicsManager::setRigidBodyVelocity(std::string rigidBodyName, float endTime, float* a){
	RigidBody* rb = rigidBodies->tableRetrieve(&rigidBodyName);
	rb->setCustomVelocity(endTime, a);
}
void PhysicsManager::applyTorqueImpulse(std::string rigidBodyName, float pitch, float yaw, float roll){
	RigidBody* rbTorque = rigidBodies->tableRetrieve(&rigidBodyName);
	if(rbTorque){
		btRigidBody* btRbTorque = rbTorque->getRigidBody();
		btRbTorque->applyTorqueImpulse(btVector3(pitch, yaw, roll));
	}
}
PhysicsManager::PhysicsManager(RenderManager* rm){
	CompareRigidBodies* compareRigidBodies = new CompareRigidBodies();
	std::cout << "Constructing tableavl" << std::endl;
	rigidBodies = new TableAVL<RigidBody, std::string>(compareRigidBodies);
	std::cout << "Constructed tableavl" << std::endl;
	CompareCompoundShapes* compareCompoundShapes = new CompareCompoundShapes();
	compoundShapes = new TableAVL<CompoundShape, std::string>(compareCompoundShapes);

	renderManager = rm;
	std::cout << "Calling init" << std::endl;
	init();
	std::cout << "Init finished" << std::endl;
	BulletDebugDrawer* debugDrawer = new BulletDebugDrawer(rm);
	std::cout << "Problem with debug drawer? Nope?" << std::endl;
	dynamicsWorld->setDebugDrawer(debugDrawer);
	dynamicsWorld->getDebugDrawer()->setDebugMode(1);
	std::cout << "DebugDrawer Finished" << std::endl;
	float* gravity = new float[3];
	gravity[0] = 0;
	gravity[1] = -9.8;
	gravity[2] = 0;
	setGravity(gravity);
	std::cout << "Set gravity" << std::endl;
	delete[] gravity;
	std::cout << "Deleted gravity" << std::endl;
}
PhysicsManager::~PhysicsManager(){
	AVLTreeIterator<CompoundShape>* compoundShapeIter = compoundShapes->tableIterator();
	while(compoundShapeIter->hasNext()){
		CompoundShape* cs = compoundShapeIter->next();
		delete cs;
	}
	delete compoundShapeIter;
	delete compoundShapes;

	int numCollisionObjects = dynamicsWorld->getNumConstraints();
	btAlignedObjectArray<btCollisionObject*> rigidBodiesArray = dynamicsWorld->getCollisionWorld()->getCollisionObjectArray();
	for(int i = numCollisionObjects-1; i >= 0; i--){
		btRigidBody* rigidBody = btRigidBody::upcast(rigidBodiesArray[i]);
		dynamicsWorld->removeCollisionObject(rigidBody);

		delete rigidBody->getMotionState();
		delete rigidBody->getCollisionShape();
	}
	AVLTreeIterator<RigidBody>* rigidBodyIter = rigidBodies->tableIterator();
	while(rigidBodyIter->hasNext()){
		RigidBody* rgb = rigidBodyIter->next();
		delete rgb;
	}
	delete rigidBodyIter;
	delete rigidBodies;

	BulletDebugDrawer* debugDrawer = (BulletDebugDrawer*)dynamicsWorld->getDebugDrawer();
	delete debugDrawer;
	delete dynamicsWorld;
	delete solver;
	delete dispatcher;
	delete collisionConfiguration;
	delete overlappingPairCache;
	renderManager = NULL;
}
void PhysicsManager::init(){
	overlappingPairCache = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
}
void PhysicsManager::createRigidBodies(){
	AVLTreeIterator<CompoundShape>* it = compoundShapes->tableIterator();
	btCompoundShape* comShape = 0;
	while(it->hasNext()){
		CompoundShape* cs = it->next();
		std::string* nodeId = cs->getKey();
		SceneNodeMotion* snm = renderManager->createSceneNodeMotion(*(cs->getKey()));

		BulletSceneNodeMotionState* ms = new BulletSceneNodeMotionState(nodeId, snm, renderManager);
		btScalar* m = new btScalar(cs->getMass());
		comShape = cs->getCompoundShape();

		btVector3* li = new btVector3(0,0,0);
		comShape->calculateLocalInertia(*m, *li);
		btRigidBody::btRigidBodyConstructionInfo* rbci = new btRigidBody::btRigidBodyConstructionInfo(*m, ms, comShape, *li);
		btRigidBody* btrb = new btRigidBody(*rbci);
		btrb->setDamping(.1,.1);
		btrb->setActivationState(DISABLE_DEACTIVATION);
		RigidBody* rb = new RigidBody(*(cs->getKey()), btrb);
		rigidBodies->tableInsert(rb);
		dynamicsWorld->addRigidBody(btrb);
	}
	delete it;
}
void PhysicsManager::createCollisionShape(std::string& compoundShapeId, std::string& collisionShape, float mass, float* trans, float* rot,float* collisionShapeParams){
	btCollisionShape* colShape = NULL;
	if(collisionShape == "Sphere"){
		colShape = new btSphereShape(btScalar(collisionShapeParams[0]));
	}
	else if(collisionShape == "cylinder_y"){
		colShape = new btCylinderShape(btVector3(collisionShapeParams[0], collisionShapeParams[1], collisionShapeParams[2]));
	}
	else if(collisionShape == "cylinder_z"){
		colShape = new btCylinderShape(btVector3(collisionShapeParams[0], collisionShapeParams[1], collisionShapeParams[2]));
	}
	else{
		colShape = new btBoxShape(btVector3(collisionShapeParams[0], collisionShapeParams[1], collisionShapeParams[2]));
	}
	btTransform* tr = new btTransform();
	tr->setIdentity();

	tr->setOrigin(btVector3(trans[0], trans[1], trans[2]));
	tr->setRotation(btQuaternion(rot[0], rot[1], rot[2], rot[3]));

	CompoundShape* cs = compoundShapes->tableRetrieve(&compoundShapeId);
	if(cs == NULL){
		compoundShapes->tableInsert(new CompoundShape(compoundShapeId, new btCompoundShape()));
		cs = compoundShapes->tableRetrieve(&compoundShapeId);
	}
	cs->addChildShape(tr, colShape, mass);
}
void PhysicsManager::updateRigidBodies(){
	int numCollisionObjects = dynamicsWorld->getNumConstraints();
	btAlignedObjectArray<btCollisionObject*> rigidBodies = dynamicsWorld->getCollisionWorld()->getCollisionObjectArray();
	for(int i=numCollisionObjects-1; i >= 0; i--){
		btRigidBody* rigidBody = btRigidBody::upcast(rigidBodies[i]);
		BulletSceneNodeMotionState* motionState = (BulletSceneNodeMotionState*) rigidBody;
		btTransform currentTransform;
		motionState->copyNodeTransformIntoBulletTransform();
	}
}
void PhysicsManager::setDebugDrawer(bool debugMode){
	dynamicsWorld->getDebugDrawer()->setDebugMode(debugMode);
}
void PhysicsManager::updateRigidBodyVelocity(float timeInc){
	AVLTreeIterator<RigidBody>* rigidBodyIter = rigidBodies->tableIterator();
	while(rigidBodyIter->hasNext()){
		RigidBody* rb = rigidBodyIter->next();
		rb->incrementCustomVelocity(timeInc);
	}
	delete rigidBodyIter;
}
void PhysicsManager::applyImpulse(std::string& rigidBodyName, float ns, float ew,float other){
	btVector3 impulse(ns, ew, other);
	btVector3 rel_pos(0,0,0);
	RigidBody* rbTorque = rigidBodies->tableRetrieve(&rigidBodyName);
	if(rbTorque){
		btRigidBody* btRbTorque = rbTorque->getRigidBody();
		btRbTorque->applyImpulse(impulse, rel_pos);
	}
}
// btRigidBody* PhysicsManager::LoadMeshFromObj(std::string collisionFile, float* pos, float* scale, float* rot){
//
// 	printf("[INFO] Obj loaded: Extracted %d verticed from obj file [%s]\n", glmesh->m_numvertices, fileName);
// 	const GLInstanceVertex& v = glmesh->m_vertices->at(0);
// 	btConvexHullShape* shape = new btConvexHullShape((const btScalar*)(&(v.xyzw[0])), glmesh->m_numvertices, sizeof(GLInstanceVertex));
// 	btVector3 color(1,1,1);
// 	btVector3 scaling(scale[0], scale[1], scale[2]);
// 	shape->setLocalScaling(scaling);
// 	if (m_options & OptimizeConvexObj)
// 	{
// 		shape->optimizeConvexHull();
// 	}
// 	if (m_options & ComputePolyhedralFeatures)
// 	{
// 		shape->initializePolyhedralFeatures();
// 	}
// 	m_collisionShapes.push_back(shape);
// 	btTransform startTransform;
// 	startTransform.setIdentity();
// 	btScalar mass(1.f);
// 	btVector3 position(pos[0], pos[1], pos[2]);
// 	startTransform.setOrigin(position);
// 	btRigidBody* body = createRigidBody(mass,startTransform,shape);
// 	btTransform tr;
// 	tr.setIdentity();
// 	btQuaternion quat;
// 	quat.setEuler(rot[0],rot[1],rot[2]); //or quat.setEulerZYX depending on the ordering you want
// 	tr.setRotation(quat);
// 	RigidBody* rb = new RigidBody(*(cs->getKey()), body);
// 	rigidBody->setCenterOfMassTransform(tr);
// 	rigidBodies->tableInsert(body);
// 	dynamicsWorld->addRigidBody(rigidBody);
// }
