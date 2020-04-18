#ifndef _TRIANGLE_BASE_H_
#define _TRIANGLE_BASE_H_

class TriangleBase
{
public:
	TriangleBase(float x1, float y1, float x2, float y2, float x3, float y3);
	void Update(float dt);
	void Draw() const;
	void SetColor(float r, float g, float b, float a = 1.0f) { m_red = r; m_green = g; m_blue = b; m_alpha = a; }
	void SetPointOnePosition(float x, float y) { m_x1 = x; m_y1 = y; }
	void SetPointTwoPosition(float x, float y) { m_x2 = x; m_y2 = y; }
	void SetPointThreePosition(float x, float y) { m_x3 = x; m_y3 = y; }
	float GetMidX()  const { return m_mid_x; }
	float GetMidY()  const { return m_mid_y; }
private:
	float m_x1 = 0.0F;
	float m_y1 = 0.0F;
	float m_x2 = 0.0F;
	float m_y2 = 0.0F;
	float m_x3 = 0.0F;
	float m_y3 = 0.0F;
	float m_mid_x = 0.0F;
	float m_mid_y = 0.0F;
	float m_red = 1.0f;
	float m_green = 1.0f;
	float m_blue = 1.0f;
	float m_alpha = 1.0f;
};

#endif
