#include "includes.h"

bool Hooks::ShouldDrawParticles( ) {
	return g_hooks.m_client_mode.GetOldMethod< ShouldDrawParticles_t >( IClientMode::SHOULDDRAWPARTICLES )( this );
}

bool Hooks::ShouldDrawFog( ) {
	// remove fog.
	if( g_menu.main.visuals.nofog.get( ) && !g_menu.main.visuals.FogOverride.get() )
		return false;

	return g_hooks.m_client_mode.GetOldMethod< ShouldDrawFog_t >( IClientMode::SHOULDDRAWFOG )( this );
}

void Hooks::OverrideView( CViewSetup* view ) {
	// damn son.
	g_cl.m_local = g_csgo.m_entlist->GetClientEntity< Player* >( g_csgo.m_engine->GetLocalPlayer( ) );

	// g_grenades.think( );
	g_visuals.ThirdpersonThink( );

    // call original.
	g_hooks.m_client_mode.GetOldMethod< OverrideView_t >( IClientMode::OVERRIDEVIEW )( this, view );

    // remove scope edge blur.
	if( g_menu.main.visuals.noscope.get( ) ) {
		if( g_cl.m_local && g_cl.m_local->m_bIsScoped( ) )
            view->m_edge_blur = 0;
	}
}

/*bool Hooks::CreateMove( float time, CUserCmd* cmd ) {
	Stack   stack;
	bool    ret;

	// let original run first.
	ret = g_hooks.m_client_mode.GetOldMethod< CreateMove_t >( IClientMode::CREATEMOVE )( this, time, cmd );

	// called from CInput::ExtraMouseSample -> return original.
	if( !cmd->m_command_number )
		return ret;

	// if we arrived here, called from -> CInput::CreateMove
	// call EngineClient::SetViewAngles according to what the original returns.
	if( ret )
		g_csgo.m_engine->SetViewAngles( cmd->m_view_angles );

	// random_seed isn't generated in ClientMode::CreateMove yet, we must set generate it ourselves.
	cmd->m_random_seed = g_csgo.MD5_PseudoRandom( cmd->m_command_number ) & 0x7fffffff;

	// get bSendPacket off the stack.
	g_cl.m_packet = stack.next( ).local( 0x1c ).as< bool* >( );

	// get bFinalTick off the stack.
	g_cl.m_final_packet = stack.next( ).local( 0x1b ).as< bool* >( );

	// invoke move function.
	g_cl.OnTick( cmd );

	return false;
}*/

bool Hooks::CreateMove(float time, CUserCmd* cmd) {
    Stack   stack;
    bool    ret;

    // let original run first.
    ret = g_hooks.m_client_mode.GetOldMethod< CreateMove_t >(IClientMode::CREATEMOVE)(this, time, cmd);

    // called from CInput::ExtraMouseSample -> return original.
    if (!cmd->m_command_number)
        return ret;

    // if we arrived here, called from -> CInput::CreateMove
    // call EngineClient::SetViewAngles according to what the original returns.
    if (ret)
        g_csgo.m_engine->SetViewAngles(cmd->m_view_angles);
    /*
    bool old_tickbase = g_tickbase.m_shift_data.m_should_attempt_shift;
    g_tickbase.m_shift_data.m_should_attempt_shift = GetAsyncKeyState(g_menu.main.movement.crimwalk.get());
    if (GetAsyncKeyState(g_menu.main.movement.crimwalk.get())) {

        // g_csgo.m_engine->ExecuteClientCmd("lastinv");
        g_cl.m_cmd->m_tick = INT_MAX;
        g_cl.m_cmd->m_command_number += 3 * 150.f;
        g_csgo.m_net->m_out_seq += 3.f * 150.f;
        g_cl.m_goal_shift = 9;

        if (g_tickbase.m_shift_data.m_should_attempt_shift)
            g_tickbase.m_shift_data.m_needs_recharge = g_cl.m_goal_shift;
        else
            g_tickbase.m_shift_data.m_needs_recharge = 0;

        g_tickbase.m_shift_data.m_did_shift_before = false;
    }
    */
    // random_seed isn't generated in ClientMode::CreateMove yet, we must set generate it ourselves.
    cmd->m_random_seed = g_csgo.MD5_PseudoRandom(cmd->m_command_number) & 0x7fffffff;

    // get bSendPacket off the stack.
    g_cl.m_packet = stack.next().local(0x1c).as< bool* >();

    // get bFinalTick off the stack.
    g_cl.m_final_packet = stack.next().local(0x1b).as< bool* >();

    // invoke move function.
    g_cl.OnTick(cmd);

    return false;
}

bool Hooks::DoPostScreenSpaceEffects(CViewSetup* setup) {
    g_visuals.RenderGlow();

    return g_hooks.m_client_mode.GetOldMethod< DoPostScreenSpaceEffects_t >(IClientMode::DOPOSTSPACESCREENEFFECTS)(this, setup);
}