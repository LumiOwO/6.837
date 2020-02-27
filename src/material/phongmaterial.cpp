#include "phongmaterial.h"
#include "glCanvas.h"
#include <GL/glut.h>

#ifdef SPECULAR_FIX
// OPTIONAL:  global variable allows (hacky) communication
// with glCanvas::display
extern int SPECULAR_FIX_WHICH_PASS;
#endif

PhongMaterial::PhongMaterial(
		const Vec3f &diffuseColor,
		const Vec3f &specularColor,
		float exponent,
		const Vec3f &reflectiveColor,
		const Vec3f &transparentColor,
		float indexOfRefraction):
	Material(diffuseColor)
{
	this->specularColor = specularColor;
	this->exponent = exponent;
	this->reflectiveColor = reflectiveColor;
	this->transparentColor = transparentColor;
	this->indexOfRefraction = indexOfRefraction;
}

Vec3f PhongMaterial::Shade(const Ray &ray, const Hit &hit,
						   const Vec3f &dirToLight, const Vec3f &lightColor) const
{
	Vec3f v = -1 * ray.getDirection();
	Vec3f half = (v + dirToLight).Normalize();

	float coe_diffuse = max2(dirToLight.Dot3(hit.getNormal()), 0);
	float coe_specular = powf(max2(half.Dot3(hit.getNormal()), 0), exponent);

	Vec3f color = getDiffuseColor() * coe_diffuse
				  + getSpecularColor() * coe_specular;

	return color * lightColor;

}

void PhongMaterial::glSetMaterial() const
{
	GLfloat specular[4] = {
		getSpecularColor().r(),
		getSpecularColor().g(),
		getSpecularColor().b(),
		1.0
	};
	GLfloat diffuse[4] = {
		getDiffuseColor().r(),
		getDiffuseColor().g(),
		getDiffuseColor().b(),
		1.0
	};

	// NOTE: GL uses the Blinn Torrance version of Phong...
	float glexponent = exponent;
	if (glexponent < 0) glexponent = 0;
	if (glexponent > 128) glexponent = 128;

	#if !SPECULAR_FIX

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);

	#else

	// OPTIONAL: 3 pass rendering to fix the specular highlight
	// artifact for small specular exponents (wide specular lobe)

	GLfloat one[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat zero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	if (SPECULAR_FIX_WHICH_PASS == 0) {
		// First pass, draw only the specular highlights
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, zero);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);

	} else if (SPECULAR_FIX_WHICH_PASS == 1) {
		// Second pass, compute normal dot light
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, one);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
	} else {
		// Third pass, add ambient & diffuse terms
		assert (SPECULAR_FIX_WHICH_PASS == 2);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
	}

	#endif
}
