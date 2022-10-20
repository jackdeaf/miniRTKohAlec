/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Koh <Koh@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 23:12:51 by Koh               #+#    #+#             */
/*   Updated: 2022/10/11 23:16:33 by Koh              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAYTRACER_H
# define RAYTRACER_H

# include "structs.h"
# include <math.h>

typedef struct s_hitpayload
{
	bool		hit;
	float		nearest_dist;
	t_vec3		point;
	t_vec3		normal;
	t_material	material;
}				t_hitpayload;

typedef struct s_light_trace
{
	float		specular_light_intensity;
	t_vec3		diffuse_light_intensity;
	t_vec3		dir;
}	t_light_trace;

t_vec3		vadd(t_vec3 a, t_vec3 b);
t_vec3		vsub(t_vec3 a, t_vec3 b);
t_vec3		vmul(t_vec3 a, t_vec3 b);
 // same as dot
float		mulvv(t_vec3 a, t_vec3 b);
t_vec3		mulvf(t_vec3 a, float b);
t_vec3		negate(t_vec3 v);
float		norm(t_vec3 v);
t_vec3		normalized(t_vec3 v);
float		vlenf(t_vec3 v);

void		nearest_cylinder(const t_vec3 orig, t_vec3 dir,
				const t_object *cy, t_hitpayload *payload);
void		nearest_sphere(const t_vec3 orig, const t_vec3 dir,
				const t_object *s, t_hitpayload *payload);
void		nearest_plane(const t_vec3 orig, const t_vec3 dir,
				const t_object *plane, t_hitpayload *payload);
void		nearest_plane2(const t_vec3 orig, const t_vec3 dir,
				const t_object *plane, t_hitpayload *payload);
void		nearest_plane3(const t_vec3 orig, const t_vec3 dir,
				const t_object *plane, t_hitpayload *payload);
t_vec3		cast_ray(const t_vec3 orig, t_vec3 dir,
				const int depth, const t_app *app);

#endif
