#ifndef G_SHADER_H
#define G_SHADER_H

#include "PhotonBox/resource/Shader.h"
#include "PhotonBox/core/Resources.h"
#include "PhotonBox/component/Camera.h"

class GShader : public InstancedShader<GShader>
{
public:
	std::string getFilePath() override
	{
		return std::string(Resources::ENGINE_RESOURCES + "/shaders/deferre-rendering/gShader");
	}

	void update(Transform* transform)
	{
		Vector4f eyePos = Vector4f(Camera::getMainCamera()->transform->getPositionWorld(), 1);
		//Matrix4f mvp = Camera::getMainCamera()->getViewProjection() * transform->getTransformationMatrix();
		Matrix4f mvp = Camera::getMainCamera()->getViewProjectionJittered() * transform->getTransformationMatrix();
		Matrix4f mvpOld = Camera::getMainCamera()->_viewProjectionLast * transform->getTransformationMatrix();
		
		glUniformMatrix4fv(uniforms["mvp"], 1, GL_FALSE, &(mvp(0, 0)));
		glUniformMatrix4fv(uniforms["mvpOld"], 1, GL_FALSE, &(mvpOld(0, 0)));
		glUniformMatrix4fv(uniforms["model"], 1, GL_FALSE, &(transform->getTransformationMatrix()(0, 0)));
		glUniformMatrix4fv(uniforms["view"], 1, GL_FALSE, &(Camera::getMainCamera()->getViewMatrix()(0, 0)));
		glUniform3fv(uniforms["viewPos"], 1, &(eyePos.x()));
	}

	void addUniforms() override
	{
		addUniform("model");
		addUniform("view");
		addUniform("mvp");
		addUniform("mvpOld");

		addUniform("viewPos");
		addUniform("minBound");
		addUniform("maxBound");
		addUniform("boundPos");
		addUniform("useCorrection");
		addUniform("emissionIntensity");

		addTexture("irradianceMap");
		addTexture("convolutedSpecularMap");

		addTexture("albedoMap");
		addTexture("roughnessMap");
		addTexture("metallicMap");
		addTexture("normalMap");
		addTexture("emissionMap");
		//addTexture("aoMap");
	}

	void addAttributes() override
	{
		addAttribut("aPos", Vertex::AttibLocation::POSITION);
		addAttribut("aNormal", Vertex::AttibLocation::NORMAL);
		addAttribut("aTexCoords", Vertex::AttibLocation::TEXTURECOORD);
		addAttribut("tangent", Vertex::AttibLocation::TANGENT);
	}

	Type getType() override
	{
		return Shader::Type::SURFACE_SHADER;
	}
};

#endif // G_SHADER_H