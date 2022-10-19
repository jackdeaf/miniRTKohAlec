/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gui.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Koh <Koh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/16 19:17:26 by Koh               #+#    #+#             */
/*   Updated: 2022/10/19 16:49:08 by leng-chu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <time.h>
#include "miniRT.h"

static const char	*get_object_typename(t_object *object)
{
	const char *const		name[OBJECT_TYPE_COUNT] = {
		[AMBIENT] = "Ambient (ratio)", [CAMERA] = "Camera (FOV)", [LIGHT] = "Light (ratio)",
		[SPHERE] = "Sphere (radius)", [PLANE] = "Plane", [CYLINDER] = "Cylinder (radius, height)",
		[CONE] = "Cone (radius, height)", [LIGHT_BONUS] = "Light bonus (ratio)"};

	if (object != NULL && object->type < END_OF_OBJECT_TYPE && name[object->type])
		return (name[object->type]);
	return "Unknown";
}

static void	reload_scene(t_app *app)
{
	ft_lstclear(&app->objects, free);
	ft_bzero(app->object_count, sizeof(app->object_count));
	parse_file(app->scene_file, app);
	app->selected_object = NULL;
	select_next(app);
	app->render_mode = DEFAULT_RENDER;
}

int	ft_checklight(t_object * const object)
{
	if (object->type == AMBIENT
		|| object->type == LIGHT
		|| object->type == LIGHT_BONUS)
		return (1);
	return (0);
}

int	adjust_object(unsigned int key, t_app *app)
{
	t_object * const	object = as_object(app->selected_object);
	const t_object_params	a[OBJECT_TYPE_COUNT] = {
		[AMBIENT] = {0.1f, 0.0f, 1.0f, 0, 0, 0},
		[CAMERA] = {10.0f, 0.0f, 180.0f, 0, 0, 0},
		[LIGHT] = {0.1f, 0.0f, 1.0f, 0, 0, 0},
		[LIGHT_BONUS] = {0.1f, 0.0f, 1.0f, 0, 0, 0},
		[SPHERE] = {0.5f, 0.0f, 1000.0f, 0, 0, 0},
		[CYLINDER] = {0.5f, 0.0f, 1000.0f, 0.5f, 0.0f, 1000.0f},
	};

	if (key == KEY_I || key == KEY_O)
		object->coor.z += pow(-1, key != KEY_I) * 0.5f;
	else if (key == KEY_UP || key == KEY_DOWN)
		object->coor.y += pow(-1, key != KEY_UP) * 0.5f;
	else if (key == KEY_RIGHT || key == KEY_LEFT)
		object->coor.x += pow(-1, key != KEY_RIGHT) * 0.5f;
	else if (key >= KEY_ONE && key <= KEY_FOUR)
	{
		if (ft_checklight(object) && (key == KEY_TWO || key == KEY_ONE))
			object->param1 += pow(-1, key != KEY_ONE) * a[object->type].param1_step;
		else if (!ft_checklight(object) && (key == KEY_THREE || key == KEY_FOUR))
			object->param1 += pow(-1, key != KEY_THREE) * a[object->type].param1_step;
		if (object->param1 < a[object->type].param1_min)
			object->param1 = a[object->type].param1_min;
		else if (object->param1 > a[object->type].param1_max)
			object->param1 = a[object->type].param1_max;
	}
	else if (key == KEY_FIVE || key == KEY_SIX)
	{
		object->param2 += pow(-1, key != KEY_FIVE) * a[object->type].param2_step;
			if (object->param2 < a[object->type].param2_min)
			object->param2 = a[object->type].param2_min;
		else if (object->param2 > a[object->type].param2_max)
			object->param2 = a[object->type].param2_max;
	}
	else
		return (0);
	return (1);
}

int	gui_input(unsigned int key, t_app *app)
{
	if (key == KEY_ESC)
		app_exit(app, NULL);
	else if (key == KEY_R)
		reload_scene(app);
	else if (key == KEY_W && ++app->mini)
		create_window(app, 600 + (app->mini & 1) * 1000, 340 + (app->mini & 1) * 560);
	else if (key == KEY_E)
		return (export_scene(app->objects), 0);
	else if (key == KEY_TAB)
	{
		select_next(app);
		mlx_put_image_to_window(app->mlx_ptr, app->win_ptr, app->image.ptr, 0, 0);
		mlx_string_put(app->mlx_ptr, app->win_ptr, 24, 24, 0XFFFF00,
			(char *)get_object_typename(app->selected_object->content));
		return (0);
	}
	else if (key == KEY_G)
		app->use_gamma_correction = !app->use_gamma_correction;
	else if (key == KEY_S || key == KEY_X)
		app->object_ptr[CAMERA]->orientation.x += pow(-1, key != KEY_S) * PI * 2 / 16;
	else if (key == KEY_D || key == KEY_C)
		app->object_ptr[CAMERA]->orientation.y += pow(-1, key != KEY_D) * PI * 2 / 16;
	else if (key == KEY_F || key == KEY_V)
		app->object_ptr[CAMERA]->orientation.z += pow(-1, key != KEY_F) * PI * 2 / 16;
	else if (key == KEY_Y)
		app->render_mode = (app->render_mode + 1) % RENDER_MODE_END;
	else if (!adjust_object(key, app))
		return (printf("unknown key %d\n", key), 0);
	++app->last_updated;
	return (0);
}

// float dir_x=0,dir_y=0,dir_z= +-1;
// rotate_x(&dir_y, &dir_z, app->camera->orientation.x);
// rotate_y(&dir_x, &dir_z, app->camera->orientation.y);
// rotate_z(&dir_x, &dir_y, app->camera->orientation.z);
// app->camera->coor.x += dir_x;
// app->camera->coor.y += dir_y;
// app->camera->coor.z += dir_z;


static void	display(t_object *c)
{
	printf("type: %s\n", get_object_typename(c));
	printf("x: %f\t y: %f\t z: %f\n", c->coor.x, c->coor.y, c->coor.z);
	printf("ox: %f\t oy: %f\t oz: %f\n", c->orientation.x, c->orientation.y, c->orientation.z);
}

int	gui_render(t_app *app)
{
	static unsigned int		last_updated = 0;
	const clock_t			begin = clock();

	if (app->last_updated == 0)
		++app->last_updated;
	if (last_updated < app->last_updated)
	{
		last_updated = app->last_updated;
		display(app->selected_object->content);
		mlx_put_image_to_window(
		app->mlx_ptr, app->win_ptr, raytrace(app), 0, 0);
		printf("raytracing %fs\n", ((double)(clock() - begin) / CLOCKS_PER_SEC));
		// printf("raytracing %ffps\n", 1.0 / ((double)(clock() - begin) / CLOCKS_PER_SEC));
		display(app->selected_object->content);
		mlx_string_put(app->mlx_ptr, app->win_ptr, 16, 24, 0XFFFF00,
			(char *)get_object_typename(app->selected_object->content));
		// mlx_string_put(app->mlx_ptr, app->win_ptr, 24, app->height - 30,
		// 	0xFFFF00,
		// 	"TAB=Next_Object  UP=Move_Y+  DOWN=Move_Y-  LEFT=Move_X-  "
		// 	"RIGHT=Move_X+  A=Move_Z+  Z=Move_Z-  N=Toggle_Render  R=Reload_Scene");
	}
	return (0);
}
