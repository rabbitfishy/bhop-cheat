#pragma once
#include "../../sdk/math/vec3.hpp"
#include "../interfaces/i_trace.hpp"

extern void SetBeamCreationAllowed(bool state);
extern bool BeamCreationAllowed(void);

//-----------------------------------------------------------------------------
// beam flags
//-----------------------------------------------------------------------------


class C_Beam;
class Beam_t;

//-----------------------------------------------------------------------------
// Purpose: Popcorn trail for Beam Follow rendering...
//-----------------------------------------------------------------------------
enum TE_Point
{
	TE_BEAMPOINTS = 0x00,		// beam effect between two points
	TE_SPRITE = 0x01,	// additive sprite, plays 1 cycle
	TE_BEAMDISK = 0x02,	// disk that expands to max radius over lifetime
	TE_BEAMCYLINDER = 0x03,		// cylinder that expands to max radius over lifetime
	TE_BEAMFOLLOW = 0x04,		// create a line of decaying beam segments until entity stops moving
	TE_BEAMRING = 0x05,		// connect a beam ring to two entities
	TE_BEAMSPLINE = 0x06,
	TE_BEAMRINGPOINT = 0x07,
	TE_BEAMLASER = 0x08,		// Fades according to viewpoint
	TE_BEAMTESLA = 0x09,
};
enum Flags_Beam
{
	FBEAM_STARTENTITY = 0x00000001,
	FBEAM_ENDENTITY = 0x00000002,
	FBEAM_FADEIN = 0x00000004,
	FBEAM_FADEOUT = 0x00000008,
	FBEAM_SINENOISE = 0x00000010,
	FBEAM_SOLID = 0x00000020,
	FBEAM_SHADEIN = 0x00000040,
	FBEAM_SHADEOUT = 0x00000080,
	FBEAM_ONLYNOISEONCE = 0x00000100,		// Only calculate our noise once
	FBEAM_NOTILE = 0x00000200,
	FBEAM_USE_HITBOXES = 0x00000400,		// Attachment indices represent hitbox indices instead when this is set.
	FBEAM_STARTVISIBLE = 0x00000800,		// Has this client actually seen this beam's start entity yet?
	FBEAM_ENDVISIBLE = 0x00001000,		// Has this client actually seen this beam's end entity yet?
	FBEAM_ISACTIVE = 0x00002000,
	FBEAM_FOREVER = 0x00004000,
	FBEAM_HALOBEAM = 0x00008000,		// When drawing a beam with a halo, don't ignore the segments and endwidth
	FBEAM_REVERSED = 0x00010000,
	NUM_BEAM_FLAGS = 17	// KEEP THIS UPDATED!
};
struct BeamTrail_t
{
	// NOTE:  Don't add user defined fields except after these four fields.
	BeamTrail_t* next;
	float			die;
	vec3_t			org;
	vec3_t			vel;
};

//-----------------------------------------------------------------------------
// Data type for beams.
//-----------------------------------------------------------------------------
struct BeamInfo_t
{
	int			m_nType;

	// Entities
	player_t* m_pStartEnt;
	int			m_nStartAttachment;
	player_t* m_pEndEnt;
	int			m_nEndAttachment;

	// Points
	vec3_t		m_vecStart;
	vec3_t		m_vecEnd;

	int			m_nModelIndex;
	const char* m_pszModelName;

	int			m_nHaloIndex;
	const char* m_pszHaloName;
	float		m_flHaloScale;

	float		m_flLife;
	float		m_flWidth;
	float		m_flEndWidth;
	float		m_flFadeLength;
	float		m_flAmplitude;

	float		m_flBrightness;
	float		m_flSpeed;

	int			m_nStartFrame;
	float		m_flFrameRate;

	float		m_flRed;
	float		m_flGreen;
	float		m_flBlue;

	bool		m_bRenderable;

	int			m_nSegments;

	int			m_nFlags;

	// Rings
	vec3_t		m_vecCenter;
	float		m_flStartRadius;
	float		m_flEndRadius;

	BeamInfo_t()
	{
		m_nType = TE_BEAMPOINTS;
		m_nSegments = -1;
		m_pszModelName = NULL;
		m_pszHaloName = NULL;
		m_nModelIndex = -1;
		m_nHaloIndex = -1;
		m_bRenderable = true;
		m_nFlags = 0;
	}
};

//-----------------------------------------------------------------------------
// Purpose: Declare client .dll beam entity interface
//-----------------------------------------------------------------------------

class IViewRenderBeams
{
public:
	// Construction
public:
	IViewRenderBeams(void);
	virtual				~IViewRenderBeams(void);

	// Implement IViewRenderBeams
public:
	virtual	void		InitBeams(void);
	virtual	void		ShutdownBeams(void);
	virtual	void		ClearBeams(void);

	// Updates the state of the temp ent beams
	virtual void		UpdateTempEntBeams();

	virtual void		DrawBeam(Beam_t* pbeam);
	virtual void		DrawBeam(C_Beam* pbeam, i_trace_filter* pEntityBeamTraceFilter = NULL);

	virtual	void		KillDeadBeams(player_t* pDeadEntity);

	virtual	void		CreateBeamEnts(int startEnt, int endEnt, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float endWidth, float fadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b, int type = -1);
	virtual Beam_t* CreateBeamEnts(BeamInfo_t& beamInfo);

	virtual	void		CreateBeamEntPoint(int	nStartEntity, const vec3_t* pStart, int nEndEntity, const vec3_t* pEnd,
		int modelIndex, int haloIndex, float haloScale,
		float life, float width, float endWidth, float fadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b);
	virtual Beam_t* CreateBeamEntPoint(BeamInfo_t& beamInfo);

	virtual	void		CreateBeamPoints(vec3_t& start, vec3_t& end, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float endWidth, float fadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b);
	virtual	Beam_t* CreateBeamPoints(BeamInfo_t& beamInfo);

	virtual	void		CreateBeamRing(int startEnt, int endEnt, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float endWidth, float fadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b, int flags);
	virtual Beam_t* CreateBeamRing(BeamInfo_t& beamInfo);

	virtual void		CreateBeamRingPoint(const vec3_t& center, float start_radius, float end_radius, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float m_nEndWidth, float m_nFadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b, int flags);
	virtual Beam_t* CreateBeamRingPoint(BeamInfo_t& beamInfo);

	virtual	void		CreateBeamCirclePoints(int type, vec3_t& start, vec3_t& end,
		int modelIndex, int haloIndex, float haloScale, float life, float width,
		float endWidth, float fadeLength, float amplitude, float brightness, float speed,
		int startFrame, float framerate, float r, float g, float b);
	virtual Beam_t* CreateBeamCirclePoints(BeamInfo_t& beamInfo);

	virtual	void		CreateBeamFollow(int startEnt, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float endWidth, float fadeLength, float r, float g, float b,
		float brightness);
	virtual Beam_t* CreateBeamFollow(BeamInfo_t& beamInfo);
};
